#ifndef CAMELOT_H
#define CAMELOT_H

// RELEASE.MAJOR.MINOR.PATCH
#define CAMELOT_VERSION "2.2.0.0"

// --- MODULES ---
#include "camelot/io.h"
#include "camelot/memory.h"
#include "types/primitives.h"
#include "types/string.h"
#include "ds/list.h"
#include "ds/table.h"

// --- SMART SCOPES ---

static inline void _cleanup_arena_func(Arena *a) {
    if (a) arena.release(a);
}

// For embedded safety/ergonomics. Auto-releases the Arena when the scope ends.
// Usage:
// ```
// scoped Arena a = arena.create(1024);
// ```
#define scoped __attribute__((cleanup(_cleanup_arena_func)))

// --- ALLOW UNSAFE ---
#ifndef ALLOW_UNSAFE
      // 1. MEMORY MANAGEMENT
      #pragma GCC poison malloc calloc realloc free aligned_alloc
      
      // 2. STRING MANIPULATION
      #pragma GCC poison strcpy strncpy strcat strncat strtok gets
      
      // 3. UNSAFE I/O
      #pragma GCC poison sprintf vsprintf scanf fscanf sscanf printf fprintf vprintf
#endif

#endif