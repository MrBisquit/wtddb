#include <wtddb/db.h>

// All these do is convert between the file versions, and the memory versions

// DB metadata
db_metadata_t wtddb_c_db_md_ftm(struct db_metadata data) {
    db_metadata_t tmp = {
        data.db_ver,

        data.num_tables,

        data.schema_begin,
        data.index_begin,
        data.table_begin
    };

    return tmp;
}

struct db_metadata wtddb_c_db_md_mtf(db_metadata_t data) {
    struct db_metadata tmp = {
        data.db_ver,

        data.num_tables,

        data.schema_begin,
        data.index_begin,
        data.table_begin
    };

    return tmp;
}

// DB config
db_config_t wtddb_c_db_c_ftm(struct db_config data) {
    db_config_t tmp = {
        data.write_journal
    };

    return tmp;
}

struct db_config wtddb_c_db_c_mtf(db_config_t data) {
    struct db_config tmp = {
        data.write_journal
    };

    return tmp;
}