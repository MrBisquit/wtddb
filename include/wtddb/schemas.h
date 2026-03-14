#ifndef WTDDB_SCHEMAS_H
#define WTDDB_SCHEMAS_H

#include <stdint.h>

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

#pragma pack(pop)

// Unpacked structures

#endif // WTDDB_SCHEMAS_H