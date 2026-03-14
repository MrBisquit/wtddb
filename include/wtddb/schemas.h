#ifndef WTDDB_SCHEMAS_H
#define WTDDB_SCHEMAS_H

#include <stdint.h>

typedef struct db_t db_t;

// This file contains structures, and methods for schemas
// See the README for more information on how this is set out

#pragma pack(push, 1)
// Packed structures
struct db_schema_head {
    // This is basically another header containing information
    // such as how many schema items there are, and what table
    // this is linked to
    //
    // However, this is only linked like this (the table head
    // links back to this) purely for ease, yes it means that
    // the data needs to be updated in 2 places, but it speeds
    // searching up later on

    uint32_t linked_table;
    uint32_t num_items;
};

struct db_schema_item_int {
    int value, min, max, def;
};

struct db_schema_item_float {
    float value, min, max, def;
};

struct db_schema_item_double {
    double value, min, max, def;
};

struct db_schema_item {
    // Bit packing again, see db.h for more information
    uint8_t primary_key     : 1;
    uint8_t secondary_key   : 1;
    uint8_t unique          : 1;
    uint8_t content_type    : 3; // Can represent 0-7 options
    uint8_t uses_min_max    : 1;
    uint8_t has_default     : 1;

    union {
        struct db_schema_item_int      item_int;
        struct db_schema_item_float    item_float;
        struct db_schema_item_double   item_double;
    } content;
};

#pragma pack(pop)

// Unpacked structures
typedef struct db_schema_head_t {
    uint32_t linked_table;
    uint32_t num_items;
} db_schema_head_t;

typedef struct db_schema_item_int_t {
    int value, min, max, def;
} db_schema_item_int_t;

typedef struct db_schema_item_float_t {
    float value, min, max, def;
} db_schema_item_float_t;

typedef struct db_schema_item_double_t {
    double value, min, max, def;
} db_schema_item_double_t;

typedef struct db_schema_item_t {
    uint8_t primary_key;
    uint8_t secondary_key;
    uint8_t unique;
    uint8_t content_type;
    uint8_t uses_min_max;
    uint8_t has_default;

    union {
        db_schema_item_int_t      item_int;
        db_schema_item_float_t    item_float;
        db_schema_item_double_t   item_double;
    } content;
} db_schema_item_t;

typedef struct db_schema_t {
    db_schema_head_t head;
    db_schema_item_t item;
} db_schema_t;

// Attempts to load all of the schemas in the database file
int  wtddb_schemas_load_all(db_t** db);
void wtddb_schemas_unload_all(db_t* db); // This does NOT save the schemas
int  wtddb_schemas_save_all(db_t* db);

#endif // WTDDB_SCHEMAS_H