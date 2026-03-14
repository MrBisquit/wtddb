#include <memory.h>
#include <wtddb/db.h>
//#include <wtddb/schemas.h>

// All these do is convert between the file versions, and the memory versions

// DB metadata
db_metadata_t wtddb_c_db_md_ftm(struct db_metadata data) {
    db_metadata_t tmp;/* = {
        data.db_ver,

        data.num_tables,

        data.schema_begin,
        data.index_begin,
        data.table_begin
    };*/

    // Experimental
    memcpy(&tmp, &data, sizeof(tmp));

    return tmp;
}

struct db_metadata wtddb_c_db_md_mtf(db_metadata_t data) {
    struct db_metadata tmp;/* = {
        data.db_ver,

        data.num_tables,

        data.schema_begin,
        data.index_begin,
        data.table_begin
    };*/

    // Experimental
    memcpy(&tmp, &data, sizeof(tmp));

    return tmp;
}

// DB config
db_config_t wtddb_c_db_c_ftm(struct db_config data) {
    db_config_t tmp;/* = {
        data.write_journal,
        data.delete_journal,
        data.clear_journal
    };*/

    // Experimental
    memcpy(&tmp, &data, sizeof(tmp));

    return tmp;
}

struct db_config wtddb_c_db_c_mtf(db_config_t data) {
    struct db_config tmp;/* = {
        data.write_journal,
        data.delete_journal,
        data.clear_journal
    };*/

    // Experimental
    memcpy(&tmp, &data, sizeof(tmp));

    return tmp;
}

// DB schema metadata
db_schema_metadata_t wtddb_c_db_smd_ftm(struct db_schema_metadata data) {
    db_schema_metadata_t tmp;
    memcpy(&tmp, &data, sizeof(tmp));

    return tmp;
}

struct db_schema_metadata wtddb_c_db_smd_mtf(db_schema_metadata_t data) {
    struct db_schema_metadata tmp;
    memcpy(&tmp, &data, sizeof(tmp));

    return tmp;
}

// DB index metadata
db_indexes_metadata_t wtddb_c_db_imd_ftm(struct db_indexes_metadata data) {
    db_indexes_metadata_t tmp;
    memcpy(&tmp, &data, sizeof(tmp));

    return tmp;
}

struct db_indexes_metadata wtddb_c_db_imd_mtf(db_indexes_metadata_t data) {
    struct db_indexes_metadata tmp;
    memcpy(&tmp, &data, sizeof(tmp));

    return tmp;
}

// DB tables metadata
db_tables_metadata_t wtddb_c_db_tmd_ftm(struct db_tables_metadata data) {
   db_tables_metadata_t tmp;
    memcpy(&tmp, &data, sizeof(tmp));

    return tmp;
}

struct db_tables_metadata wtddb_c_db_tmd_mtf(db_tables_metadata_t data) {
    struct db_tables_metadata tmp;
    memcpy(&tmp, &data, sizeof(tmp));

    return tmp;
}