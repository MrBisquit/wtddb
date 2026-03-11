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

    // Push the changes to the file
    wtddb_push_db(db);
}

void wtddb_open_db(const char* path, db_t* db) {
    // Always assume that db is a NULL pointer

    db = malloc(sizeof(db_t));
    db->file_path = malloc(sizeof(&path));
    strcpy(db->file_path, path);
    db->stream = fopen(db->file_path, "wb+"); // wb+ means read + write with
                                              // binary, in this context
    
    // Now to actually load the contents
    fseek(db->stream, 0, SEEK_SET); // Send to beginning

    size_t read = 0;

    struct db_metadata tmp_metadata;
    read = fread(&tmp_metadata, sizeof(struct db_metadata), 1, db->stream);

    // Before we go any further, check if tmp_metadata.db_ver is 0
    if(tmp_metadata.db_ver == 0 || read == 0) {
        // The database isn't set up
        wtddb_create_db(db);

        // Simply return, wtddb_create_db will handle loading metadata into
        // memory
        return;
    }

    // Convert the file data to memory data
    db->metadata = wtddb_c_db_md_ftm(tmp_metadata);
}

void wtddb_close_db(db_t* db) {
    // Write out any changes, close file stream, then dealloc (free) db
    // If you use the db variable after calling this, it will cause
    // undefined behaviour, as you are attempting to access memory that
    // does not exist

    // Write out headers
    wtddb_push_db(db);

    // Close file, and free memory
    fclose(db->stream);
    free(db); // The variable db is NULL at this point
}

void wtddb_push_db(db_t* db) {
    // This doesn't actually write out ALL of the metadata, only the main
    // chunks, like the whole db metadata

    // Write out headers
    fseek(db->stream, 0, SEEK_SET); // Send to beginning

    // Convert the metadata first
    struct db_metadata tmp_metadata = wtddb_c_db_md_mtf(db->metadata);

    // Write the db metadata
    fwrite(&tmp_metadata, sizeof(tmp_metadata), 1, db->stream);
}

int wtddb_db_status(db_t* db) {
    if(db->stream == NULL)
        return 1; // Not open

    return 0; // Ready
}