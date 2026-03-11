#include <wtddb/db.h>

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