#ifndef WTDDB_DB_H
#define WTDDB_DB_H

#include <stdint.h> // For things like uint64_t
#include <stdio.h>  // For FILE
#include <getline.h> // For ssize_t
//#include <ansi_console_v2.h> // For ANSI colours

#include <wtddb/schemas.h>

// Definitions
#define WTDDB_DB_VER                        1    // Something that isn't 1,
                                                 // this will be important later
#define WTDDB_DB_MAX_TABLE_NAME_LENGTH      64
#define WTDDB_DB_MAX_COLUMN_NAME_LENGTH     64
#define WTDDB_DB_MAX_INDEX_NAME_LENGTH      64
#define WTDDB_DB_MAX_COLUMNS                1024 // That's a lot of possible columns

// Log definitons
#define WTDDB_INFO(fmt, ...) \
    do { printf("\x1B[94minfo:\033[0m " fmt "\n", __VA_ARGS__); } while(0)
#define WTDDB_WARN(fmt, ...) \
    do { printf("\x1B[93mwarn:\033[0m " fmt "\n", __VA_ARGS__); } while(0)
#define WTDDB_ERROR(fmt, ...) \
    do { printf("\x1B[91merror:\033[0m " fmt "\n", __VA_ARGS__); } while(0)
#define WTDDB_CRITICAL(fmt, ...) \
    do { printf("\x1B[31mcrit:\033[0m " fmt "\n", __VA_ARGS__); } while(0)

// Basic file structure (different from data)
// These are basically squished together, which is NOT optimal for memory,
// but IS optimal for secondary
#pragma pack(push, 1) // 1-byte alignment

struct db_metadata {
    uint32_t db_ver; // This should never be 0

    uint32_t num_tables;

    // Pointers to the start of the hashtable and datatables
    uintptr_t schema_begin; // Schema data begin
    uintptr_t index_begin;  // Index data begin
    uintptr_t table_begin;  // Table data begin
};

struct db_config {
    // Bit packing in C, the ": 1" here means only take up 1 bit
    //
    // For bit packing, if we didn't use bit packing, the data would look like
    // 00000001 00000000 00000001 (3 bytes for 3 booleans)
    // That data WITH bit packing
    // 00000101 (1 byte for 3 booleans)
    // That saves on potentially up to 7 bytes
    //
    // Journals basically allow you to recover from mistakes, or corruptions
    // This simply writes to the same path, but with an additional ".jrnl" at the end,
    // this file will be deleted, or may be left depending on the below options
    uint8_t write_journal   : 1;
    uint8_t delete_journal  : 1;
    uint8_t clear_journal   : 1;
};

struct db_index_metadata {
    uintptr_t table_begin;  // The actual hashtable for each data table
};

struct db_table_metadata {
    char name[WTDDB_DB_MAX_TABLE_NAME_LENGTH];
    uint32_t num_columns; // Might need 64-bit in the future
};

struct db_schema_metadata {
    uint32_t total_schemas;
};

struct db_indexes_metadata {
    uint32_t total_indexes; // At least 1 index per table
};

// Technically there's metadata for each table, and this is just the overall
// metadata, providing links to all of the tables
struct db_tables_metadata {
    uint32_t total_tables;
};

#pragma pack(pop)

// Actual data structure (in memory)
// These aren't squished together, this actually lets the compiler optimise
// based on pages, it uses more memory, but is faster
typedef struct db_metadata_t {
    uint32_t db_ver;

    uint32_t num_tables;

    uintptr_t schema_begin;
    uintptr_t index_begin;
    uintptr_t table_begin;
} db_metadata_t;

typedef struct db_config_t {
    uint8_t write_journal;
    uint8_t delete_journal;
    uint8_t clear_journal;
} db_config_t;

typedef struct db_index_metadata_t {
    uintptr_t table_begin;
} db_index_metadata_t;

typedef struct db_table_metadata_t {
    char name[WTDDB_DB_MAX_TABLE_NAME_LENGTH];
    uint32_t num_columns;
} db_table_metadata_t;

typedef struct db_schema_metadata_t {
    uint32_t total_schemas;
} db_schema_metadata_t;

typedef struct db_indexes_metadata_t {
    uint32_t total_indexes;
} db_indexes_metadata_t;

typedef struct db_tables_metadata_t {
    uint32_t total_tables;
} db_tables_metadata_t;

typedef struct db_t {
    char* file_path;
    FILE* stream;

    db_metadata_t metadata;
    db_config_t config;

    db_schema_metadata_t schema_metadata;
    db_indexes_metadata_t indexes_metadata;
    db_tables_metadata_t tables_metadata;

    uint32_t num_schemas_loaded;
    db_schema_t* schemas;
} db_t;

// Functions
// Create the database
void wtddb_create_db(db_t* db);
// Open the database
void wtddb_open_db(const char* path, db_t** db);
// Close the database
void wtddb_close_db(db_t* db);
// Push metadata changes to the file
int  wtddb_push_db(db_t* db);
// Get the status of the database
// If this is anything other than 0, the db is not ready
int  wtddb_db_status(db_t* db);

// Conversion functions
// DB metadata
db_metadata_t wtddb_c_db_md_ftm(struct db_metadata data); // File -> Memory
struct db_metadata wtddb_c_db_md_mtf(db_metadata_t data); // Memory -> File

// DB config
db_config_t wtddb_c_db_c_ftm(struct db_config data); // File -> Memory
struct db_config wtddb_c_db_c_mtf(db_config_t data); // Memory -> File

// DB schema metadata
db_schema_metadata_t wtddb_c_db_smd_ftm(struct db_schema_metadata data); // File -> Memory
struct db_schema_metadata wtddb_c_db_smd_mtf(db_schema_metadata_t data); // Memory -> File

// DB indexes metadata
db_indexes_metadata_t wtddb_c_db_imd_ftm(struct db_indexes_metadata data); // File -> Memory
struct db_indexes_metadata wtddb_c_db_imd_mtf(db_indexes_metadata_t data); // Memory -> File

// DB tables metadata
db_tables_metadata_t wtddb_c_db_tmd_ftm(struct db_tables_metadata data); // File -> Memory
struct db_tables_metadata wtddb_c_db_tmd_mtf(db_tables_metadata_t data); // Memory -> File

// REPL structures and functions
typedef struct {
    char* buffer;
    size_t buffer_length;
    ssize_t input_length;
} repl_buffer_t;

repl_buffer_t* repl_new_buffer();
void repl_read_input(repl_buffer_t* buffer);
void repl_dump_db(db_t* db);
void repl_dump_db_raw(db_t* db);

#endif // WTDDB_DB_H