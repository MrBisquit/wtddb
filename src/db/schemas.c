#include <memory.h>
#include <wtddb/db.h>
#include <wtddb/schemas.h>

int wtddb_schemas_load_all(db_t** db) {
    (*db)->num_schemas_loaded = 0;


    return 0;
}

void wtddb_schemas_unload_all(db_t* db) {
    uint32_t loaded = db->num_schemas_loaded;
    WTDDB_INFO("Unloading %d loaded schemas", loaded);

    free(db->schemas);

    WTDDB_INFO("Successfully unloaded %d schemas", loaded);
    db->num_schemas_loaded = 0;
}

int wtddb_schemas_save_all(db_t* db) {
    if(db->num_schemas_loaded <= 0) {
        WTDDB_INFO("There were no schemas to push (%d loaded)", db->num_schemas_loaded);
        return 1;
    }

    return 0;
}