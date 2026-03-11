#ifndef WTDDB_DB_H
#define WTDDB_DB_H

#include <stdint.h> // For things like uint64_t
#include <stdio.h>  // For FILE
#include <getline.h> // For ssize_t
//#include <ansi_console_v2.h> // For ANSI colours

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

struct db_index_metadata {
    uintptr_t table_begin;  // The actual hashtable for each data table
};

struct db_table_metadata {
    char name[WTDDB_DB_MAX_TABLE_NAME_LENGTH];
    uint32_t num_columns; // Might need 64-bit in the future
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

typedef struct db_index_metadata_t {
    uintptr_t table_begin;
} db_index_metadata_t;

typedef struct db_table_metadata_t {
    char name[WTDDB_DB_MAX_TABLE_NAME_LENGTH];
    uint32_t num_columns;
} db_table_metadata_t;

typedef struct db_t {
    char* file_path;
    FILE* stream;

    db_metadata_t metadata;
} db_t;

// Functions
// Create the database
void wtddb_create_db(db_t* db);
// Open the database
void wtddb_open_db(const char* path, db_t* db);
// Close the database
void wtddb_close_db(db_t* db);
// Push metadata changes to the file
void wtddb_push_db(db_t* db);
// Get the status of the database
// If this is anything other than 0, the db is not ready
int  wtddb_db_status(db_t* db);

// Conversion functions
db_metadata_t wtddb_c_db_md_ftm(struct db_metadata data); // File -> Memory
struct db_metadata wtddb_c_db_md_mtf(db_metadata_t data); // Memory -> File

// REPL structures and functions
typedef struct {
    char* buffer;
    size_t buffer_length;
    ssize_t input_length;
} repl_buffer_t;

repl_buffer_t* repl_new_buffer();
void repl_read_input(repl_buffer_t* buffer);

#endif // WTDDB_DB_H