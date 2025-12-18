#ifndef CAMELOT_STRING_H
#define CAMELOT_STRING_H

#include "primitives.h"

// --- TYPES ---

typedef struct {
      u8 *ptr;
      u64 len;
} String;

// --- NAMESPACE ---

typedef struct {
      // Wraps a null-terminated C-string into a Camelot String.
      //
      // Usage: 
      // ```
      // String s = string.from("Hello");
      // ```
      String (*from)(const char *c_str);

      // Checks if two strings are identical in content and length.
      bool (*equal)(String a, String b);
} StringNamespace;

extern const StringNamespace string;

#endif