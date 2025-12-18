#ifndef CAMELOT_TABLE_H
#define CAMELOT_TABLE_H

#include "../camelot/memory.h"
#include "../types/string.h"

// A single Key-Value pair
typedef struct {
      String key;
      void  *value;
      bool   alive; // Is this slot occupied?
} Entry;

typedef struct {
      Arena *source;
      Entry *entries;
      u64    cap;    // Total slots
      u64    count;  // Occupied slots
} Table;

// --- NAMESPACE ---

typedef struct {
      // Creates a new Hash Table.
      // Usage: Table cfg = table.create(&arena, 64);
      Table (*create)(Arena *a, u64 capacity);

      // Maps a String key to a value pointer.
      // Overwrites if key exists. Auto-resizes if 75% full.
      // Usage: table.put(&cfg, string.from("Width"), &w);
      void (*put)(Table *t, String key, void *value);

      // Retrieves a value. Returns NULL if not found.
      // Usage: int *w = table.get(&cfg, string.from("Width"));
      void* (*get)(Table *t, String key);
} TableNamespace;

extern const TableNamespace table;

#endif