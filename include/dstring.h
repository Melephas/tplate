#pragma once


#include <libdbuf.h>


/* Represents a dynamically allocated and managed string
 */
typedef struct dstring_s string_t;


// Create a new empty string
string_t* string_new();

// Create a new string from a C string
string_t* string_from(char* c_string);

// Destroy a string when it's no longer needed
void string_destroy(string_t* string);

// Append a character to the end of the string
void string_append_char(string_t* string, char c);

// Append an entire C string to the end of the string
void string_append_cstring(string_t* string, char* cstring);

// Append a managed string to the end of the string
void string_append_string(string_t* string, string_t* string1);

// Creates an exact copy of the string
string_t* string_duplicate(string_t* string);

// Creates a C string from the string
char* string_to_cstring(string_t* string);

// I'm not doing these right now
//string_t* string_substring(string_t* string, uint64_t start, uint64_t length);
//char string_character_at(string_t* string, uint64_t index);