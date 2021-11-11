#pragma once


/* Represents a file and its contents. Each file should be added to a directory or directly to a project in order to
 * represent where it should be located in the project.
 *
 * See directory_t for more details.
 */
typedef struct file_s file_t;


// ==== Creation and destruction ====

// Creates a new, empty instance of a file.
file_t* new_file();
// Destroys a file instance when it's no longer needed.
void file_destroy(file_t* file);


// ==== File properties ====

// Sets the name of the file.
void file_set_name(file_t* file, char* name);

// Gets the name of the file.
char* file_get_name(file_t* file);

// Sets the contents of the file.
void file_set_content(file_t* file, char* content);

// Gets the contents of the file.
char* file_get_content(file_t* file);


// ==== Utility functions ====

// Creates and writes the file in the current directory.
void file_write(file_t* project);