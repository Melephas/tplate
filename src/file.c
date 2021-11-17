#include "../include/file.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "libdbuf.h"


struct file_s {
    dbuf_t* name;
    dbuf_t* contents;
};


// ==== Creation and destruction ====

// Creates a new, empty instance of a file.
file_t* new_file() {
    file_t* ret = calloc(1, sizeof(file_t));
    ret->name = dbuf_create(10);
    ret->contents = dbuf_create(10);
    return ret;
}

// Destroys a file instance when it's no longer needed.
void file_destroy(file_t* file) {
    dbuf_destroy(file->name);
    dbuf_destroy(file->contents);
    free(file);
}


// ==== File properties ====

// Sets the name of the file.
void file_set_name(file_t* file, dbuf_t* name) {
    dbuf_destroy(file->name);
    file->name = name;
}

// Gets the name of the file.
dbuf_t* file_get_name(file_t* file) {
    return file->name;
}

// Sets the contents of the file.
void file_set_contents(file_t* file, dbuf_t* contents) {
    dbuf_destroy(file->contents);
    file->contents = contents;
}

// Gets the contents of the file.
dbuf_t* file_get_contents(file_t* file) {
    return file->contents;
}


// ==== Utility functions ====

// Creates and writes the file in the current directory.
void file_write(file_t* file) {
    uint8_t* filename_seq = NULL;
    uint8_t* contents_seq = NULL;
    dbuf_as_bytes(file->name, filename_seq);
    dbuf_as_bytes(file->contents, contents_seq);

    size_t len_offset = sizeof(uint64_t) / sizeof(uint8_t);

    uint8_t* filename = &filename_seq[len_offset];
    uint64_t filename_len = ((uint64_t*) filename_seq)[0];
    uint8_t* contents = &contents_seq[len_offset];
    uint64_t contents_len = ((uint64_t*) contents_seq)[0];

    size_t c_filename_len = contents_len * sizeof(uint8_t) + sizeof(char);
    char* c_filename = calloc(c_filename_len, 1);

    // Copy the bytes from the dbuf to a normal C string
    memcpy(c_filename, filename, filename_len);

    FILE* f = fopen(c_filename, "w");
    fwrite(contents, sizeof(uint8_t), contents_len, f);
    fclose(f);
}