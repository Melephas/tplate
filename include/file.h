#pragma once


#include <stdint.h>
#include <dstring.h>


/* Represents a file and its contents. Each file should be added to a directory or directly to a project in order to
 * represent where it should be located in the project.
 *
 * See directory for more details.
 */
typedef struct file_s file;


// ==== Creation and destruction ====

// Creates a new, empty instance of a file.
file* file_new();
// Destroys a file instance when it's no longer needed.
void file_destroy(file* file);


// ==== File properties ====

// Sets the name of the file.
void file_set_name(file* file, string* name);

// Gets the name of the file.
string* file_get_name(file* file);

// Sets the contents of the file.
void file_set_contents(file* file, string* contents);

// Gets the contents of the file.
string* file_get_contents(file* file);


// ==== Utility functions ====

// Creates and writes the file in the current directory.
void file_write(file* file);