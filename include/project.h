#pragma once


#include "file.h"
#include "directory.h"


/* Represents a full project as a collection of files and directories. Every file and directory stored directly in the
 * project is considered to be at the 'root' of the project. Subdirectories and files in other directories should be
 * added to the relevant directory object.
 *
 * See file and directory for more details.
 */
typedef struct project_s project;


enum project_type {
    project_type_none,
    project_type_shared_library,
    project_type_static_library,
    project_type_library,
    project_type_executable
};


// ==== Creation and destruction ====

// Creates a new, empty instance of a project.
project* project_new(enum project_type type);
// Destroys a project instance when it's no longer needed.
void project_destroy(project* project);

// ==== Adding files and folders to the project ====

// Sets the name of the project
void project_set_name(project* p, string* name);
// Add a file to the root of the project.
void project_add_file(project* project, file* file);
// Add a directory to the root of the project.
void project_add_directory(project* project, directory* directory);

// ==== Utility functions ====

// Generates the structures need for a default project
void project_generate_structure(project* p);
// Creates all the folders and writes all the files in the project.
void project_write_all(project* project);