#include <malloc.h>
#include <string.h>
#include <wtddb/db.h>

// This is literally entirely just for opening, closing, and creating databases
// Databases are created automatically if they do not already exists
// DO NOT manually call create_db, open_db will call it for you
//
// You must ALWAYS close the database before exiting, even if there was an
// unexpected error. Not doing this could lead to database corruption, and I
// don't think anyone would be too happy about that

void wtddb_create_db(db_t* db) {
    // Assume the db is in fact NOT a NULL pointer

    WTDDB_INFO("The database did not exist, so it has been created", "");

    // Sets the db->metadata.db_ver value to be what ever the WTDDB_DB_VER
    // is currently set to, this will be important later for versioning
    db->metadata.db_ver = WTDDB_DB_VER;
    db->config.write_journal = 1;
    db->config.delete_journal = 0;
    db->config.clear_journal = 0;

    // Write data to the file for the first time, all of the headers
    // will be next to one another

    // Move cursor to start of file
    fseek(db->stream, 0, SEEK_SET);

    // Convert all of the data to their packed form
    struct db_metadata          tmp_metadata            = wtddb_c_db_md_mtf(db->metadata);
    struct db_config            tmp_config              = wtddb_c_db_c_mtf(db->config);
    struct db_schema_metadata   tmp_schema_metadata     = wtddb_c_db_smd_mtf(db->schema_metadata);
    struct db_indexes_metadata  tmp_indexes_metadata    = wtddb_c_db_imd_mtf(db->indexes_metadata);
    struct db_tables_metadata   tmp_tables_metadata     = wtddb_c_db_tmd_mtf(db->tables_metadata);

    // Write out the DB metadata
    fwrite(&tmp_metadata, sizeof(tmp_metadata), 1, db->stream);
    fwrite(&tmp_config, sizeof(tmp_config), 1, db->stream);
    fwrite(&tmp_schema_metadata, sizeof(tmp_schema_metadata), 1, db->stream);
    fwrite(&tmp_indexes_metadata, sizeof(tmp_indexes_metadata), 1, db->stream);
    fwrite(&tmp_tables_metadata, sizeof(tmp_tables_metadata), 1, db->stream);

    // Update positions
    db->metadata.schema_begin = sizeof(tmp_metadata) + sizeof(tmp_config);
    db->metadata.index_begin = db->metadata.schema_begin + sizeof(tmp_schema_metadata);
    db->metadata.table_begin = db->metadata.index_begin + sizeof(tmp_indexes_metadata);

    // Push the changes to the file
    wtddb_push_db(db);
}

void wtddb_open_db(const char* path, db_t** db) {
    // Always assume that db is a NULL pointer

    *db = malloc(sizeof(db_t));
    (*db)->file_path = malloc(strlen(path) + 1);
    strcpy((*db)->file_path, path);
    (*db)->stream = fopen(path, "wb+"); // wb+ means read + write with
                                        // binary, in this context
    
    // Now to actually load the contents
    fseek((*db)->stream, 0, SEEK_SET); // Send to beginning

    size_t read = 0;

    struct db_metadata tmp_metadata;
    read = fread(&tmp_metadata, sizeof(tmp_metadata), 1, (*db)->stream);

    // Before we go any further, check if anything is actually read
    if(read == 0) {
        // The database isn't set up
        wtddb_create_db((*db));

        // Simply return, wtddb_create_db will handle loading metadata into
        // memory
        return;
    }

    struct db_config tmp_config;
    read = fread(&tmp_config, sizeof(tmp_config), 1, (*db)->stream);

    // Convert the file data to memory data
    (*db)->metadata = wtddb_c_db_md_ftm(tmp_metadata);
    (*db)->config = wtddb_c_db_c_ftm(tmp_config);

    // Load schema, indexes, and tables metadata
    // Schema metadata
    fseek((*db)->stream, (*db)->metadata.schema_begin, SEEK_SET);
    struct db_schema_metadata tmp_schema_metadata;
    read = fread(&tmp_schema_metadata, sizeof(tmp_schema_metadata), 1, (*db)->stream);
    (*db)->schema_metadata = wtddb_c_db_smd_ftm(tmp_schema_metadata);

    // Indexes metadata
    fseek((*db)->stream, (*db)->metadata.index_begin, SEEK_SET);
    struct db_indexes_metadata tmp_indexes_metadata;
    read = fread(&tmp_indexes_metadata, sizeof(tmp_indexes_metadata), 1, (*db)->stream);
    (*db)->indexes_metadata = wtddb_c_db_imd_ftm(tmp_indexes_metadata);

    // Tables metadata
    fseek((*db)->stream, (*db)->metadata.table_begin, SEEK_SET);
    struct db_tables_metadata tmp_tables_metadata;
    read = fread(&tmp_tables_metadata, sizeof(tmp_tables_metadata), 1, (*db)->stream);
    (*db)->tables_metadata = wtddb_c_db_tmd_ftm(tmp_tables_metadata);
}

void wtddb_close_db(db_t* db) {
    // Write out any changes, close file stream, then dealloc (free) db
    // If you use the db variable after calling this, it will cause
    // undefined behaviour, as you are attempting to access memory that
    // does not exist

    if(db->stream == NULL) {
        WTDDB_WARN("Database already closed", "");
        return;
    }

    WTDDB_INFO("Closing database", "");

    // Write out headers
    if(wtddb_push_db(db) != 0) {
        WTDDB_CRITICAL("Could not push headers to database", "");
        return;
    }

    WTDDB_INFO("Closing database file and freeing memory", "");

    // Close file, and free memory
    fclose(db->stream);
    free(db); // The variable db is dangling, using it will cause undefined behaviour
}

int wtddb_push_db(db_t* db) {
    // This doesn't actually write out ALL of the metadata, only the main
    // chunks, like the whole db metadata

    if(db->stream == NULL) {
        WTDDB_CRITICAL("The database file is not open", "");

        return 1;
    }

    // Write out headers

    // Send to beginning
    if(fseek(db->stream, 0, SEEK_SET) != 0) {
        WTDDB_CRITICAL("File seeking failed", "");

        return 1;
    }

    size_t write_total = 0;
    size_t write = 0;

    // Convert the metadata first
    struct db_metadata tmp_metadata = wtddb_c_db_md_mtf(db->metadata);

    // Write the db metadata
    write = fwrite(&tmp_metadata, sizeof(tmp_metadata), 1, db->stream);
    write_total += write;
    if(write == 0) {
        WTDDB_CRITICAL("Failed to write to database file, the database is now corrupted", "");
        WTDDB_INFO("Wrote %zd elements", write_total);

        return 1;
    }

    // Convert the config next
    struct db_config tmp_config = wtddb_c_db_c_mtf(db->config);

    // Write the db config
    write = fwrite(&tmp_config, sizeof(tmp_config), 1, db->stream);
    write_total += write;
    if(write == 0) {
        WTDDB_CRITICAL("Failed to write to database file, the database is now corrupted", "");
        WTDDB_INFO("Wrote %zd elements", write_total);

        return 1;
    }

    /// @todo Write db_schema_metadata, db_indexes_metadata, and db_tables_metadata
    // Load schema, indexes, and tables metadata
    fseek(db->stream, db->metadata.schema_begin, SEEK_SET);
    struct db_schema_metadata tmp_schema_metadata = wtddb_c_db_smd_mtf(db->schema_metadata);
    write = fwrite(&tmp_schema_metadata, sizeof(tmp_schema_metadata), 1, db->stream);
    write_total += write;
    if(write == 0) {
        WTDDB_CRITICAL("Failed to write to database file, the database is now corrupted", "");
        WTDDB_INFO("Wrote %zd elements", write_total);

        return 1;
    }

    fseek(db->stream, db->metadata.index_begin, SEEK_SET);
    struct db_indexes_metadata tmp_indexes_metadata = wtddb_c_db_imd_mtf(db->indexes_metadata);
    write = fwrite(&tmp_indexes_metadata, sizeof(tmp_indexes_metadata), 1, db->stream);
    write_total += write;
    if(write == 0) {
        WTDDB_CRITICAL("Failed to write to database file, the database is now corrupted", "");
        WTDDB_INFO("Wrote %zd elements", write_total);

        return 1;
    }

    fseek(db->stream, db->metadata.table_begin, SEEK_SET);
    struct db_tables_metadata tmp_tables_metadata = wtddb_c_db_tmd_mtf(db->tables_metadata);
    write = fwrite(&tmp_tables_metadata, sizeof(tmp_tables_metadata), 1, db->stream);
    write_total += write;
    if(write == 0) {
        WTDDB_CRITICAL("Failed to write to database file, the database is now corrupted", "");
        WTDDB_INFO("Wrote %zd elements", write_total);

        return 1;
    }

    WTDDB_INFO("Wrote %zd elements", write_total);

    return 0;
}

int wtddb_db_status(db_t* db) {
    if(db->stream == NULL)
        return 1; // Not open

    return 0; // Ready
}