#include <stdlib.h>
#include <list.h>

#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <stdio.h>

#include "directory.h"
#include "log.h"


struct directory_s {
    string* name;
    list* subdirs;
    list* files;
};

static void write_subdir(void* subdir);
static void write_file(void* filePointer);

// ==== Creation and destruction ====

// Creates a new, empty instance of a directory.
directory* directory_new() {
    directory* ret = malloc(sizeof(directory));
    ret->name = string_new();
    ret->subdirs = list_new();
    ret->files = list_new();

    return ret;
}

// Destroys a directory instance when it's no longer needed.
void directory_destroy(directory* dir) {
    string_destroy(dir->name);
    list_destroy(dir->subdirs);
    list_destroy(dir->files);
}


// ==== Directory Properties ====

// Sets the name of the directory
void directory_set_name(directory* dir, string* name) {
    string_destroy(dir->name);
    dir->name = name;
}

// Gets the name of the directory
string* directory_get_name(directory* dir) {
    return string_duplicate(dir->name);
}


// ==== Adding files and folders to the project ====

// Add a file to this directory.
void directory_add_file(directory* dir, file* file) {
    list_append(dir->files, file);
}

// Add a subdirectory to this directory.
void directory_add_directory(directory* dir, directory* subdir) {
    list_append(dir->subdirs, subdir);
}


// ==== Utility functions ====

// Creates all the folders and writes all the files in the directory.
void directory_write_all(directory* dir) {
    if (dir->name == NULL) {
        printf("Cannot write directory with no name\n");
        exit(EXIT_FAILURE);
    }
    char* c_name = string_to_cstring(dir->name);
    debug_log("Creating directory %s\n", c_name);
    errno = 0;
    int ret = mkdir(c_name, S_IRWXU);
    if (ret == -1) {
        switch (errno) {
            case EEXIST:
                debug_log("Directory %s already exists\n", c_name);
                break;
            default:
                perror("tplate.directory.directory_write_all(1)");
                exit(EXIT_FAILURE);
                break;
        }
    }

    errno = 0;
    ret = chdir(c_name);
    if (ret == -1) {
        perror("tplate.directory.directory_write_all(2)");
        exit(EXIT_FAILURE);
    }

    list_foreach(dir->files, write_file);
    list_foreach(dir->subdirs, write_subdir);

    errno = 0;
    ret = chdir("..");
    if (ret == -1) {
        perror("tplate.directory.directory_write_all(3)");
        exit(EXIT_FAILURE);
    }
}

void write_file(void* filePointer) {
    file* f = (file*) filePointer;
    file_write(f);
}

void write_subdir(void* subdir) {
    directory* dir = (directory*) subdir;
    directory_write_all(dir);
}