#define ALLOW_UNSAFE
#include "camelot/io.h"
#include <stdio.h>

// --- INTERNAL IMPLEMENTATION ---

static u64 internal_stream(File *f, Op op, void *arg, u64 num) {
      if (!f) return 0;

      switch (op) {
            case OPEN: {
                  const char *path = (const char*)arg;
                  FILE *h = fopen(path, "rb"); 
                  
                  if (!h) {
                        f->status = FILE_NOT_FOUND;
                        f->size = 0;
                        return 0; // Fail
                  }

                  // --- PIPE FIX LOGIC ---
                  // Try to jump to end to measure size.
                  if (fseek(h, 0, SEEK_END) == 0) {
                        // Success: It's a regular file
                        long len = ftell(h);
                        fseek(h, 0, SEEK_SET);
                        f->size = (u64)len;
                  } else {
                        // Failure: It's a Pipe, Socket, or Character Device
                        // We cannot measure size, but we can still read streamingly.
                        clearerr(h); 
                        f->size = 0; // Unknown size
                  }

                  f->handle = h;
                  f->status = OK;
                  return 1; // Success
            }

            case READ: {
                  if (f->status != OK || !f->handle) return 0;
                  return fread(arg, 1, num, (FILE*)f->handle);
            }

            case SKIP: {
                  if (f->status != OK || !f->handle) return 0;
                  fseek((FILE*)f->handle, (long)num, SEEK_CUR);
                  return 0; 
            }

            case CLOSE: {
                  if (f->handle) {
                        fclose((FILE*)f->handle);
                        f->handle = NULL;
                        f->status = IO_ERROR;
                  }
                  return 0;
            }
      }
      return 0;
}

static String internal_slurp(Arena *a, const char *path) {
      File f = {0}; 
      if (!internal_stream(&f, OPEN, (void*)path, 0)) return (String){0};

      // CRITICAL: We cannot slurp pipes effectively without a resizeable array.
      // If size is 0, we fail (for now) to avoid allocation issues.
      if (f.size == 0) {
            internal_stream(&f, CLOSE, NULL, 0);
            return (String){0}; 
      }

      u8 *buf = allocate(a, f.size + 1);
      if (!buf) {
            internal_stream(&f, CLOSE, NULL, 0);
            return (String){0};
      }

      internal_stream(&f, READ, buf, f.size);
      buf[f.size] = '\0'; 

      internal_stream(&f, CLOSE, NULL, 0);

      return (String){ .ptr = buf, .len = f.size };
}

// --- LINKING EXTERNALS ---
// These functions are implemented in src/io/io.c
// We link them here to populate the namespace.
extern void put(String s);
extern void putn(String s);
extern void print(const char *fmt, ...);

// --- PUBLIC NAMESPACE ---
const IONamespace io = {
      .put    = put,
      .putn   = putn,
      .print  = print,
      .stream = internal_stream,
      .slurp  = internal_slurp
};