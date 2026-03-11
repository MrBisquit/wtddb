#include <wtddb/db.h>

// Could just do const char** argv, but this looks cooler
int main(int argc, char* const argv[]) {
    // So there's basically 2 modes to this:
    // - The default SQLite-like interface with commands that you type out
    // - The other one which allows you to just execute it without going into the interface
    //   Like wtddb dbfile.db -q "Some cool SQL query" -o text
    //
    // Oh, and capitalisation IS enforced here (because it also looks cooler)
    //
    // This works on a schema, which makes it easier to interpret the data
    // Once the initial schema is generated, new fields MUST be nullable, they
    // can be changed to non-nullable, or have a DEFAULT value set later
    // This is so that you can still access the data in the table
    //
    // There's probably more information (and maybe even diagrams) in the README.md file

    if(argc < 2) {
        WTDDB_CRITICAL("You must attach the path to a database file, e.g. \"%s database.db\"", argv[0]);

        return 1;
    }

    // Open the database
    db_t* db = NULL;
    wtddb_open_db(argv[1], db);

    // Parse any other arguments

    // Nothing to do, so start REPL (Read Eval Print Loop), commonly used in databases
    // such as sqlite

    repl_buffer_t* input_buffer = repl_new_buffer(input_buffer);

    while(1) {
        printf("wtddb> ");
        repl_read_input(input_buffer);
    }
}