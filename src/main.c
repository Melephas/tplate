#include <stdio.h>
#include <getopt.h>

#include <project.h>
#include <string.h>
#include <stdlib.h>


void write_project(string* name, enum project_type type);
void print_usage();


int main(int argc, char** argv) {
    int opt = 0;
    int long_index = 0;
    static struct option long_options[] = {
        {"name",       required_argument, 0, 'n'},
        {"library",    optional_argument, 0, 'l'},
        {"executable", no_argument,       0, 'e'},
        {0,            0,         0, 0}
    };

    string* name = NULL;
    enum project_type type = project_type_none;

    while ((opt = getopt_long(argc, argv, "n:l::e", long_options, &long_index)) != -1) {
        switch (opt) {
            case 'n':
                name = string_from(optarg);
                break;
            case 'l':
                if (type != project_type_none) {
                    printf("Cannot make a project of more than one type\n");
                    exit(EXIT_FAILURE);
                }
                if (optarg) {
                    if (strcmp(optarg, "shared") == 0) {
                        type = project_type_shared_library;
                    } else if (strcmp(optarg, "static") == 0) {
                        type = project_type_static_library;
                    } else {
                        printf("Project type '%s' not recognised\n", optarg);
                        exit(EXIT_FAILURE);
                    }
                } else {
                    type = project_type_library;
                }
                break;
            case 'e':
                if (type != project_type_none) {
                    printf("Cannot make a project of more than one type\n");
                    exit(EXIT_FAILURE);
                }
                type = project_type_executable;
                break;
            default:
                print_usage();
                exit(EXIT_FAILURE);
        }
    }

    // Check to make sure that name is set
    if (name == NULL) {
        printf("-n <name> or --name <name> is required\n");
        print_usage();
        exit(EXIT_FAILURE);
    }

    // All options were processed without error
    write_project(name, type);

    return 0;
}

void write_project(string* name, enum project_type type) {
    project* proj = project_new(type);
    project_set_name(proj, name);
    project_generate_structure(proj);
    project_write_all(proj);
    project_destroy(proj);
}

void print_usage() {
    printf("Usage: tplate -n/--name <name> [-l/--library [shared/static]] [-e/--executable]\n");
}
