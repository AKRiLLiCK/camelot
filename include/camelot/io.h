#ifndef CAMELOT_IO_H
#define CAMELOT_IO_H

#include "types/string.h"
#include "types/primitives.h"
#include "camelot/memory.h"

// --- TYPES ---

typedef struct {
      void *handle;     // Internal FILE* pointer
      Result status;    // Current state (OK, FILE_NOT_FOUND, etc.)
      u64 size;         // Total size (0 if unknown/pipe)
} File;

typedef enum {
      OPEN, READ, SKIP, CLOSE
} Op;

// --- NAMESPACE ---

typedef struct {
      // --- OUTPUT ---
      
      void (*put)(String s);
      void (*putn)(String s);
      void (*print)(const char *fmt, ...);

      // --- FILES ---

      String (*slurp)(Arena *a, const char *path);
      u64    (*stream)(File *f, Op op, void *arg, u64 num);
} IONamespace;

extern const IONamespace io;

#endif