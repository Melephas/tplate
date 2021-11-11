#include "../include/file.h"

#include <stdlib.h>


struct file_s {
    char* name;
    char* contents;
};


// ==== Creation and destruction ====

// Creates a new, empty instance of a file.
file_t* new_file() {
    file_t* ret = calloc(1, sizeof(file_t));
    return ret;
}

// Destroys a file instance when it's no longer needed.
void file_destroy(file_t* file) {
    free(file);
}


// ==== File properties ====

// Sets the name of the file.
void file_set_name(file_t* file, char* name) {
    file->name = name;
}

// Gets the name of the file.
char* file_get_name(file_t* file);

// Sets the contents of the file.
void file_set_content(file_t* file, char* content);

// Gets the contents of the file.
char* file_get_content(file_t* file);


// ==== Utility functions ====

// Creates and writes the file in the current directory.
void file_write(file_t* project);