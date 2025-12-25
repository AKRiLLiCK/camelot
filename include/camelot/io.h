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
      // --- INPUT ---

      // Reads a line from stdin (up to '\n') with a specific buffer limit.
      // Behavior:
      // - If 'cap' is 0, defaults to 4096 bytes (4KB).
      // - If 'cap' > 0, attempts to allocate exactly that amount.
      // Safety: Returns empty String if the Arena is full (OOM).
      // Usage:
      // ```
      // // Standard (4KB)
      // String cmd = io.scan(&arena, 0);
      // 
      // // Large Input
      // String prompt = io.scan(&arena, 1024 * 1024 * 50);
      // ```
      String (*scan)(Arena *a, u64 cap);

      // --- OUTPUT ---
      
      // Writes a Camelot String to stdout.
      // Usage:
      // ```
      // io.put(s);
      // ```
      void (*put)(String s);

      // Formatted output dispatcher. Supports %S (String), %s (C-str), %i (int), %f (double).
      // Usage:
      // ```
      // io.print("Hello %S, count: %i\n", name, 42);
      // ```
      void (*print)(const char *fmt, ...);

      // --- FILES ---

      // Reads an entire file into the Arena in one shot. Auto-closes the file.
      // Usage:
      // ```
      // String config = io.slurp(&arena, "settings.ini");
      // ```
      String (*slurp)(Arena *a, const char *path);

      // Low-level file stream dispatcher. 
      // Manages OPEN, READ, SKIP, CLOSE operations.
      // WARNING: You MUST call CLOSE when finished to prevent file handle leaks.
      // Usage:
      // ```
      // File f = {0};
      // io.stream(&f, OPEN, "data.bin", 0);
      // io.stream(&f, READ, buf, 10);
      // io.stream(&f, CLOSE, NULL, 0);
      // ```
      u64 (*stream)(File *f, Op op, void *arg, u64 num);
} IONamespace;

extern const IONamespace io;

#endif