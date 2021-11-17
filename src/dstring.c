#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#include "dstring.h"


#ifndef STRING_CHUNK_SIZE
    #define STRING_CHUNK_SIZE 10
#endif


struct dstring_s {
    char* characters;
    uint64_t allocated_length;
    uint64_t used_length;
};


// Create a new empty string
string_t* string_new() {
    string_t* ret = calloc(1, sizeof(string_t));
    ret->allocated_length = STRING_CHUNK_SIZE;
    ret->used_length = 0;
    ret->characters = calloc(STRING_CHUNK_SIZE, sizeof(char));

    return ret;
}

// Create a new string from a C string
string_t* string_from(char* c_string) {
    string_t* ret = string_new();
    string_append_cstring(ret, c_string);
    return ret;
}

// Destroy a string when it's no longer needed
void string_destroy(string_t* string) {
    free(string->characters);
    free(string);
}

// Append a character to the end of the string
void string_append_char(string_t* string, char c) {
    string->characters[string->used_length] = c;
    if (++(string->used_length) >= string->allocated_length) {
        string->allocated_length += STRING_CHUNK_SIZE;
        char* tmp = realloc(string->characters, string->allocated_length * sizeof(char));
        if (tmp == NULL) {
            fprintf(
                stderr,
                "Failed to reallocate dstring (%s)\n",
                strerror(errno)
            );
            exit(1);
        }
        string->characters = tmp;
    }
}

// Append an entire C string to the end of the string
void string_append_cstring(string_t* string, char* cstring) {
    uint64_t str_len = strlen(cstring);
    for (uint64_t i = 0; i < str_len; i++) {
        string_append_char(string, cstring[i]);
    }
}

// Append a managed string to the end of the string
void string_append_string(string_t* string, string_t* string1);

// Creates an exact copy of the string
string_t* string_duplicate(string_t* string);

// Creates a C string from the string
char* string_to_cstring(string_t* string) {
    char* ret = calloc(string->used_length + 1, sizeof(char));
    memcpy(ret, string->characters, string->used_length);
    return ret;
}