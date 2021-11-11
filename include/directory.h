#pragma once


#include "file.h"


/* Represents a directory as a collection of files and directories. Every file and directory stored directly in the
 * directory is considered to be inside this directory. Subdirectories and files in other directories should be added to
 * the relevant directory object.
 *
 * See file_t for more details.
 */
typedef struct directory_s directory_t;


// ==== Creation and destruction ====

// Creates a new, empty instance of a directory.
directory_t* new_directory();

// Destroys a directory instance when it's no longer needed.
void directory_destroy(directory_t* project);


// ==== Adding files and folders to the project ====

// Add a file to this directory.
void directory_add_file(directory_t* project, file_t* file);

// Add a subdirectory to this directory.
void directory_add_directory(directory_t* project, directory_t* directory);


// ==== Utility functions ====

// Creates all the folders and writes all the files in the directory.
void directory_write_all(directory_t* project);