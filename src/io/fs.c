#define ALLOW_UNSAFE
#include "camelot/io.h"
#include <stdio.h>

// --- THE DISPATCHER ---

u64 stream(File *f, Op op, void *arg, u64 num) {
      if (!f) return 0;

      switch (op) {
            case OPEN: {
                  const char *path = (const char*)arg;
                  // Default to "Read Binary" for the engine
                  FILE *h = fopen(path, "rb"); 
                  
                  if (!h) {
                        f->status = FILE_NOT_FOUND;
                        f->size = 0;
                        return 0; // Fail
                  }

                  // Smart Size Calc
                  fseek(h, 0, SEEK_END);
                  long len = ftell(h);
                  fseek(h, 0, SEEK_SET);

                  f->handle = h;
                  f->status = OK;
                  f->size   = (u64)len;
                  return 1; // Success
            }

            case READ: {
                  if (f->status != OK || !f->handle) return 0;
                  return fread(arg, 1, num, (FILE*)f->handle);
            }

            case SKIP: {
                  if (f->status != OK || !f->handle) return 0;
                  fseek((FILE*)f->handle, (long)num, SEEK_CUR);
                  return (u64)ftell((FILE*)f->handle);
            }

            case CLOSE: {
                  if (f->handle) {
                        fclose((FILE*)f->handle);
                        f->handle = NULL;
                        f->status = IO_ERROR; // Invalidate
                  }
                  return 0;
            }
      }
      return 0;
}

// --- SLURP WRAPPER ---

String slurp(Arena *a, const char *path) {
      File f = {0}; 

      // 1. OPEN
      if (!stream(&f, OPEN, (void*)path, 0)) {
            return (String){0};
      }

      // 2. ALLOC
      u8 *buf = allocate(a, f.size + 1);
      if (!buf) {
            stream(&f, CLOSE, NULL, 0);
            return (String){0};
      }

      // 3. READ
      stream(&f, READ, buf, f.size);
      buf[f.size] = '\0'; // Cap

      // 4. CLOSE
      stream(&f, CLOSE, NULL, 0);

      return (String){ .ptr = buf, .len = f.size };
}