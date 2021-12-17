#pragma once


#include "file.h"


/* Represents a directory as a collection of files and directories. Every file and directory stored directly in the
 * directory is considered to be inside this directory. Subdirectories and files in other directories should be added to
 * the relevant directory object.
 *
 * See file for more details.
 */
typedef struct directory_s directory;


// ==== Creation and destruction ====

// Creates a new, empty instance of a directory.
directory* directory_new();

// Destroys a directory instance when it's no longer needed.
void directory_destroy(directory* dir);


// ==== Directory Properties ====

// Sets the name of the directory
void directory_set_name(directory* dir, string* name);

// Gets the name of the directory
string* directory_get_name(directory* dir);


// ==== Adding files and folders to the project ====

// Add a file to this directory.
void directory_add_file(directory* dir, file* file);

// Add a subdirectory to this directory.
void directory_add_directory(directory* dir, directory* subdir);


// ==== Utility functions ====

// Creates all the folders and writes all the files in the directory.
void directory_write_all(directory* dir);