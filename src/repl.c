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
    printf("======== Begin database dump ========\n");

    printf("\n========  End database dump  ========\n");
}