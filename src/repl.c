#include <malloc.h>
#include <wtddb/db.h>
#include <getline.h>

// This file contains helper functions for the REPL (more information in main.c)

// Allocates a new buffer for the REPL input
repl_buffer_t* repl_new_buffer() {
    repl_buffer_t* buffer = malloc(sizeof(repl_buffer_t));
    if(!buffer) return NULL;

    buffer->buffer = NULL;
    buffer->buffer_length = 0;
    buffer->input_length = 0;

    return buffer;
}

// Attempts to read from the console to the REPL buffer
void repl_read_input(repl_buffer_t* buffer) {
    ssize_t read = getline(&(buffer->buffer), &(buffer->buffer_length), stdin);

    if(read <= 0) {
        WTDDB_CRITICAL("Failed to read user input", "");
        exit(1);
        return;
    }

    buffer->input_length = read - 1;
    buffer->buffer[read - 1] = 0;
}

// Dumps DB information
void repl_dump_db(db_t* db) {
    printf("======== Begin database dump ========\n\n");

    printf("Database metadata\n");
    printf("\tDatabase version: %d\n", db->metadata.db_ver);
    printf("\t     Table count: %d\n", db->metadata.num_tables);
    printf("\t    Schema begin: 0x%llX\n", db->metadata.schema_begin);
    printf("\t     Index begin: 0x%llX\n", db->metadata.index_begin);
    printf("\t     Table begin: 0x%llX\n", db->metadata.table_begin);

    printf("\nDatabase config\n");
    printf("\t   Write journal: %d\n", db->config.write_journal);
    printf("\t  Delete journal: %d\n", db->config.delete_journal);
    printf("\t   Clear journal: %d\n", db->config.clear_journal);

    printf("\nSchemas\n");
    printf("\t    Schema count: %d\n", db->schema_metadata.total_schemas);
    printf("\t          Loaded: %d\n", db->num_schemas_loaded);

    printf("\nIndexes\n");
    printf("\t    Schema count: %d\n", db->indexes_metadata.total_indexes);

    printf("\nTables\n");
    printf("\t     Table count: %d\n", db->tables_metadata.total_tables);

    printf("\n========  End database dump  ========\n");
}

void repl_dump_db_raw(db_t* db) {
    // This just makes it easier to search for patterns like 0xCDCDCDCD,
    // which usually indicate that an unsigned integer (like a uint32_t)
    // is invalid, or corrupted

    printf(
        "0x%X 0x%X 0x%llX 0x%llX 0x%llX 0x%X 0x%X 0x%X 0x%X 0x%X 0x%X 0x%X\n",
        db->metadata.db_ver,
        db->metadata.num_tables,
        db->metadata.schema_begin,
        db->metadata.index_begin,
        db->metadata.table_begin,

        db->config.write_journal,
        db->config.delete_journal,
        db->config.clear_journal,

        db->schema_metadata.total_schemas,
        db->num_schemas_loaded,

        db->indexes_metadata.total_indexes,

        db->tables_metadata.total_tables
    );
}