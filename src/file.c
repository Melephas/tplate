#include "../include/file.h"
#include "log.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


struct file_s {
    string* name;
    string* contents;
};


// ==== Creation and destruction ====

// Creates a new, empty instance of a file.
file* file_new() {
    file* ret = calloc(1, sizeof(file));
    ret->name = string_new();
    ret->contents = string_new();
    return ret;
}

// Destroys a file instance when it's no longer needed.
void file_destroy(file* file) {
    string_destroy(file->name);
    string_destroy(file->contents);
    free(file);
}


// ==== File properties ====

// Sets the name of the file.
void file_set_name(file* file, string* name) {
    string_destroy(file->name);
    file->name = name;
}

// Gets the name of the file.
string* file_get_name(file* file) {
    return string_duplicate(file->name);
}

// Sets the contents of the file.
void file_set_contents(file* file, string* contents) {
    string_destroy(file->contents);
    file->contents = contents;
}

// Gets the contents of the file.
string* file_get_contents(file* file) {
    return string_duplicate(file->contents);
}


// ==== Utility functions ====

// Creates and writes the file in the current directory. Assumes that the program is executing in the directory that the
// file is intended to be written in.
void file_write(file* file) {
    char* cname = string_to_cstring(file->name);
    char* contents = string_to_cstring(file->contents);

    debug_log("Writing %s\n", cname);

    FILE* outputFile = fopen(cname, "w");
    fwrite(contents, sizeof(char), strlen(contents), outputFile);
    fclose(outputFile);
}