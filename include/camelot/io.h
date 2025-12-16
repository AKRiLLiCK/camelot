#ifndef CAMELOT_IO_H
#define CAMELOT_IO_H

#include "types/string.h"

// --- FUNCTIONS ---

// Writes a raw string to Standard Output (console).
// Does not append a newline.
void put(String s);

// Writes a string followed by a newline character ('\n').
void putn(String s);

// The Camelot Formatter.
// Replaces 'printf'. Parses a format string and prints arguments.
// Example: print("Hello %s", str_arg);
void print(const char *fmt, ...);

#endif