#include "project.h"
#include "log.h"

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#include <list.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>


struct project_s {
    string* name;
    list* files;
    list* directories;
    enum project_type type;
};


static void generate_cmake_executable_project(project* p);
static void generate_cmake_library_project(project* p);

static void write_subdir(void* subdir);
static void write_file(void* filePointer);


// ==== Creation and destruction ====

// Creates a new, empty instance of a project.
project* project_new(enum project_type type) {
    project* ret = malloc(sizeof(project));
    ret->name = NULL;
    ret->files = list_new();
    ret->directories = list_new();
    ret->type = type;

    return ret;
}

// Destroys a project instance when it's no longer needed.
void project_destroy(project* project) {
    if (project->name) {
        string_destroy(project->name);
    }
    list_destroy(project->files);
    list_destroy(project->directories);
}


// ==== Adding files and folders to the project ====

// Add a file to the root of the project.
void project_add_file(project* project, file* file) {
    list_append(project->files, file);
}

// Add a directory to the root of the project.
void project_add_directory(project* project, directory* directory) {
    list_append(project->directories, directory);
}


// ==== Utility functions ====

// Sets the name of the project
void project_set_name(project* p, string* name) {
    if (p->name) {
        string_destroy(p->name);
    }
    p->name = name;
}

// Generates the structures need for a default project
void project_generate_structure(project* p) {
    if (p->name == NULL) {
        printf("Cannot generate a project with no name\n");
        exit(EXIT_FAILURE);
    }

    switch (p->type) {
        case project_type_executable:
            generate_cmake_executable_project(p);
            break;
        case project_type_library:
        case project_type_static_library:
        case project_type_shared_library:
            generate_cmake_library_project(p);
            break;
        case project_type_none:
        default:
            printf("Cannot generate a project with invalid type\n");
            exit(EXIT_FAILURE);
            break;
    }
}

// Creates all the folders and writes all the files in the project.
void project_write_all(project* project) {
    if (project->name == NULL) {
        printf("Cannot write project with no name\n");
        exit(EXIT_FAILURE);
    }

    char* c_name = string_to_cstring(project->name);
    debug_log("Creating directory %s\n", c_name);
    errno = 0;
    int ret = mkdir(c_name, S_IRWXU);
    if (ret == -1) {
        switch (errno) {
            case EEXIST:
                debug_log("Directory %s already exists\n", c_name);
                break;
            default:
                perror("tplate.project.project_write_all(1)");
                exit(EXIT_FAILURE);
                break;
        }
    }

    errno = 0;
    ret = chdir(c_name);
    if (ret == -1) {
        perror("tplate.project.project_write_all(2)");
        exit(EXIT_FAILURE);
    }

    list_foreach(project->files, write_file);
    list_foreach(project->directories, write_subdir);

    errno = 0;
    ret = chdir("..");
    if (ret == -1) {
        perror("tplate.project.project_write_all(3)");
        exit(EXIT_FAILURE);
    }
}

static file* generate_cmake_config_in(project* p) {
    file* cmake_config = file_new();
    file_set_name(cmake_config, string_from("Config.cmake.in"));
    string* cmake_config_content = string_from("\n@PACKAGE_INIT@\n\ninclude(\"${CMAKE_CURRENT_LIST_DIR}/");
    string_append_string(cmake_config_content, p->name);
    string_append_cstring(cmake_config_content, "Targets.cmake\")");
    file_set_contents(cmake_config, cmake_config_content);
    return cmake_config;
}

static directory* generate_cmake_dir(project* p) {
    directory* cmake_dir = directory_new();
    directory_set_name(cmake_dir, string_from("cmake"));
    directory_add_file(cmake_dir, generate_cmake_config_in(p));
    return cmake_dir;
}

static file* generate_main_c(project* p) {
    file* main_c = file_new();
    file_set_name(main_c, string_from("main.c"));
    file_set_contents(main_c, string_from(
        "#include <stdio.h>\n\nint main() {\n\tprintf(\"Hello, World!\\n\");\n\treturn 0;\n}\n"
    ));
    return main_c;
}

static directory* generate_src_dir(project* p) {
    directory* src = directory_new();
    directory_set_name(src, string_from("src"));

    if (p->type == project_type_executable) {
        directory_add_file(src, generate_main_c(p));
    } else if (p->type != project_type_none) {
        file* source_file = file_new();
        string* file_name = string_duplicate(p->name);
        string_append_cstring(file_name, ".c");
        file_set_name(source_file, file_name);
        string* file_contents = string_from("#include \"");
        string_append_string(file_contents, p->name);
        string_append_cstring(file_contents, ".h\"\n\nchar* hello() {\n\treturn \"Hello, World!\";\n}\n");
        file_set_contents(source_file, file_contents);
        directory_add_file(src, source_file);
    }
    return src;
}

static file* generate_executable_cmakelists_txt(project* p) {
    file* cmakelists = file_new();
    file_set_name(cmakelists, string_from("CMakeLists.txt"));
    string* cmakelists_contents = string_from("cmake_minimum_required(VERSION 3.10)\nproject(");
    string_append_string(cmakelists_contents, p->name);
    string_append_cstring(cmakelists_contents, " C)\n\nset(CMAKE_C_STANDARD 11)\n\ninclude_directories(include)\n\nadd_executable(");
    string_append_string(cmakelists_contents, p->name);
    string_append_cstring(cmakelists_contents," src/main.c)\n");
    file_set_contents(cmakelists, cmakelists_contents);
    return cmakelists;
}

static file* generate_library_cmakelists_txt(project* p) {
    file* cmakelists = file_new();
    file_set_name(cmakelists, string_from("CMakeLists.txt"));
    string* cmakelists_contents = string_from(
        "cmake_minimum_required(VERSION 3.13)\n"
        "project("
    );
    string_append_string(cmakelists_contents, p->name);
    string_append_cstring(cmakelists_contents,
        " C)\n"
        "\n"
        "set(CMAKE_C_STANDARD 11)\n"
        "\n"
        "set(PROJECT_VERSION 0.1.0)\n"
        "\n"
        "set(SOURCES src/"
    );
    string_append_string(cmakelists_contents, p->name);
    string_append_cstring(cmakelists_contents,
        ".c)\n"
        "\n"
        "add_compile_options(\n"
        "    -g\n"
        ")\n"
        "\n"
        "add_library(\n"
        "    "
    );
    string_append_string(cmakelists_contents, p->name);
    string_append_cstring(cmakelists_contents,
        " SHARED\n"
        "    ${SOURCES}\n"
        ")\n"
        "\n"
        "add_library(\n"
        "    "
    );
    string_append_string(cmakelists_contents, p->name);
    string_append_cstring(cmakelists_contents,
        "Static STATIC\n"
        "    ${SOURCES}\n"
        ")\n"
        "\n"
        "set_target_properties("
    );
    string_append_string(cmakelists_contents, p->name);
    string_append_cstring(cmakelists_contents,
        "Static PROPERTIES OUTPUT_NAME "
    );
    string_append_string(cmakelists_contents, p->name);
    string_append_cstring(cmakelists_contents,
        ")\n"
        "\n"
        "add_library("
    );
    string_append_string(cmakelists_contents, p->name);
    string_append_cstring(cmakelists_contents,
        "::"
    );
    string_append_string(cmakelists_contents, p->name);
    string_append_cstring(cmakelists_contents,
        " ALIAS "
    );
    string_append_string(cmakelists_contents, p->name);
    string_append_cstring(cmakelists_contents,
        ")\n"
        "add_library("
    );
    string_append_string(cmakelists_contents, p->name);
    string_append_cstring(cmakelists_contents,
        "::"
    );
    string_append_string(cmakelists_contents, p->name);
    string_append_cstring(cmakelists_contents,
        "::Static ALIAS "
    );
    string_append_string(cmakelists_contents, p->name);
    string_append_cstring(cmakelists_contents,
        "Static)\n"
        "\n"
        "target_include_directories(\n"
        "    "
    );
    string_append_string(cmakelists_contents, p->name);
    string_append_cstring(cmakelists_contents,
        "\n"
        "    PUBLIC\n"
        "    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>\n"
        "    $<INSTALL_INTERFACE:include>\n"
        "    #    PUBLIC include\n"
        ")\n"
        "\n"
        "target_include_directories(\n"
        "    "
    );
    string_append_string(cmakelists_contents, p->name);
    string_append_cstring(cmakelists_contents,
        "Static\n"
        "    PUBLIC\n"
        "    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>\n"
        "    $<INSTALL_INTERFACE:include>\n"
        ")\n"
        "\n"
        "include(GNUInstallDirs)\n"
        "\n"
        "install(\n"
        "    TARGETS "
    );
    string_append_string(cmakelists_contents, p->name);
    string_append_cstring(cmakelists_contents,
        " "
    );
    string_append_string(cmakelists_contents, p->name);
    string_append_cstring(cmakelists_contents,
        "Static\n"
        "    EXPORT "
    );
    string_append_string(cmakelists_contents, p->name);
    string_append_cstring(cmakelists_contents,
        "Targets\n"
        "    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}\n"
        ")\n"
        "\n"
        "install(\n"
        "    FILES\n"
        "    include/"
    );
    string_append_string(cmakelists_contents, p->name);
    string_append_cstring(cmakelists_contents,
        ".h\n"
        "    TYPE INCLUDE\n"
        "    COMPONENT\n"
        "    Devel\n"
        ")\n"
        "\n"
        "install(\n"
        "    EXPORT "
    );
    string_append_string(cmakelists_contents, p->name);
    string_append_cstring(cmakelists_contents,
        "Targets\n"
        "    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/"
    );
    string_append_string(cmakelists_contents, p->name);
    string_append_cstring(cmakelists_contents,
        "\n"
        "    NAMESPACE "
    );
    string_append_string(cmakelists_contents, p->name);
    string_append_cstring(cmakelists_contents,
        "::\n"
        "    FILE "
    );
    string_append_string(cmakelists_contents, p->name);
    string_append_cstring(cmakelists_contents,
        "Targets.cmake\n"
        ")\n"
        "\n"
        "include(CMakePackageConfigHelpers)\n"
        "configure_package_config_file(\n"
        "    \"cmake/Config.cmake.in\"\n"
        "    \"cmake/"
    );
    string_append_string(cmakelists_contents, p->name);
    string_append_cstring(cmakelists_contents,
        "Config.cmake\"\n"
        "    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/"
    );
    string_append_string(cmakelists_contents, p->name);
    string_append_cstring(cmakelists_contents,
        "\n"
        "    PATH_VARS\n"
        "    CMAKE_INSTALL_LIBDIR\n"
        ")\n"
        "\n"
        "write_basic_package_version_file(\n"
        "    ${CMAKE_CURRENT_BINARY_DIR}/cmake/"
    );
    string_append_string(cmakelists_contents, p->name);
    string_append_cstring(cmakelists_contents,
        "ConfigVersion.cmake\n"
        "    VERSION ${PROJECT_VERSION}\n"
        "    COMPATIBILITY SameMajorVersion\n"
        ")\n"
        "\n"
        "install(\n"
        "    FILES\n"
        "    \"${CMAKE_CURRENT_BINARY_DIR}/cmake/"
    );
    string_append_string(cmakelists_contents, p->name);
    string_append_cstring(cmakelists_contents,
        "Config.cmake\"\n"
        "    \"${CMAKE_CURRENT_BINARY_DIR}/cmake/"
    );
    string_append_string(cmakelists_contents, p->name);
    string_append_cstring(cmakelists_contents,
        "ConfigVersion.cmake\"\n"
        "    DESTINATION\n"
        "    \"${CMAKE_INSTALL_LIBDIR}/cmake/"
    );
    string_append_string(cmakelists_contents, p->name);
    string_append_cstring(cmakelists_contents,
        "\"\n"
        ")");
    file_set_contents(cmakelists, cmakelists_contents);
    return cmakelists;
}

static directory* generate_include_dir(project* p) {
    directory* ret = directory_new();
    directory_set_name(ret, string_from("include"));
    file* header = file_new();
    string* header_name = string_duplicate(p->name);
    string_append_cstring(header_name, ".h");
    file_set_name(header, header_name);
    file_set_contents(header, string_from(
            "#pragma once\n\nchar* hello();\n"
    ));
    directory_add_file(ret, header);
    return ret;
}

static void generate_cmake_executable_project(project* p) {
    project_add_directory(p, generate_cmake_dir(p));
    project_add_directory(p, generate_src_dir(p));
    project_add_file(p, generate_executable_cmakelists_txt(p));
}

static void generate_cmake_library_project(project* p) {
    project_add_directory(p, generate_cmake_dir(p));
    project_add_directory(p, generate_src_dir(p));
    project_add_directory(p, generate_include_dir(p));
    project_add_file(p, generate_library_cmakelists_txt(p));
}

static void write_file(void* filePointer) {
    file* f = (file*) filePointer;
    file_write(f);
}

static void write_subdir(void* subdir) {
    directory* dir = (directory*) subdir;
    directory_write_all(dir);
}
