#include "../include/camelot.h"
#include <stdio.h>

// --- IMPLEMENTATION: FILE SYSTEM ---

Str c_fs_read(Arena *a, const char *path) {
      // 1. Open File (Binary Mode)
      FILE *f = fopen(path, "rb");
      if (!f) {
            return (Str){ .ptr = NULL, .len = 0 };
      }

      // 2. Get File Size
      fseek(f, 0, SEEK_END);
      long length = ftell(f);
      fseek(f, 0, SEEK_SET);

      if (length < 0) {
            fclose(f);
            return (Str){ .ptr = NULL, .len = 0 };
      }

      // 3. Allocate Memory (Plus null terminator for safety)
      AllocResult res = c_alloc(a, length + 1, 1);
      if (res.error != C_OK) {
            fclose(f);
            return (Str){ .ptr = NULL, .len = 0 };
      }

      char *buffer = (char *)res.unwrap;

      // 4. Read Content
      size_t read_count = fread(buffer, 1, length, f);
      buffer[length] = '\0'; // Null-terminate
      fclose(f);

      if (read_count != (size_t)length) {
            return (Str){ .ptr = NULL, .len = 0 };
      }

      return (Str){ .ptr = buffer, .len = (size_t)length };
}

bool c_fs_write(const char *path, Str content) {
      FILE *f = fopen(path, "wb");
      if (!f) return false;

      size_t written = fwrite(content.ptr, 1, content.len, f);
      fclose(f);

      return (written == content.len);
}