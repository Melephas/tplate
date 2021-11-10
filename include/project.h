#pragma once


#include "file.h"
#include "directory.h"


/* Represents a full project as a collection of files and directories. Every file and directory stored directly in the
 * project is considered to be at the 'root' of the project. Subdirectories and files in other directories should be
 * added to the relevant directory object.
 *
 * See file_t and directory_t for more details.
 */
typedef struct project_s project_t;


// ==== Creation and destruction ====

// Creates a new, empty instance of a project.
project_t* new_project();
// Destroys a project instance when it's no longer needed.
void project_destroy(project_t* project);

// ==== Adding files and folders to the project ====

// Add a file to the root of the project.
void project_add_file(project_t* project, file_t* file);
// Add a directory to the root of the project.
void project_add_directory(project_t* project, directory_t* directory);

// ==== Utility functions ====

// Creates all the folders and writes all the files in the project.
void project_write_all(project_t* project);