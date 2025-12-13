// tests.c - Camelot Verification Suite
// Compile: gcc tests.c src/*.c -std=c2x -Wall -o run_tests

#include "include/camelot.h"

// --- TEST HARNESS ---
#define TITLE(name) printf("\n--- TEST %d: %s ---\n", ++test_count, name)
#define REQUIRE(cond, fmt, ...) \
      do { \
            if (cond) printf("      [PASS] " fmt "\n", ##__VA_ARGS__); \
            else { printf("      [FAIL] " fmt " (Line %d)\n", ##__VA_ARGS__, __LINE__); exit(1); } \
      } while(0)

int main() {
      int test_count = 0;
      printf("\nCAMELOT SYSTEM CHECK\n");

      // 1. System Init
      ArenaResult sys = c_arena_create(1024);
      REQUIRE(sys.error == C_OK, "Arena created (1024 bytes)");
      Arena *arena = sys.unwrap;

      // TEST 1: Allocation
      TITLE("Valid Allocation");
      AllocResult r1 = c_new(arena, int, 2);
      REQUIRE(r1.error == C_OK, "Allocated 2 integers");
      ((int*)r1.unwrap)[0] = 42;
      REQUIRE(((int*)r1.unwrap)[0] == 42, "Data written correctly");

      // TEST 2: OOM
      TITLE("Intentional OOM");
      AllocResult r2 = c_new(arena, char, 10000);
      REQUIRE(r2.error == C_OOM, "Blocked oversized allocation");

      // TEST 3: Reset
      TITLE("Arena Reset");
      c_new(arena, int, 100); // Fill
      REQUIRE(arena->offset > 0, "Arena is dirty (%zu bytes)", arena->offset);
      c_arena_reset(arena);
      REQUIRE(arena->offset == 0, "Offset reset to 0");
      
      AllocResult r3 = c_new(arena, int, 1);
      REQUIRE(r3.unwrap == arena->buffer, "Pointer reused start of buffer");

      // TEST 4: Scopes
      TITLE("Manual Scopes");
      c_arena_reset(arena);
      size_t base = arena->offset;
      {
            TempArena tmp = c_temp_begin(arena);
            c_new(arena, double, 50);
            REQUIRE(arena->offset > base, "Offset moved forward");
            c_temp_end(tmp);
      }
      REQUIRE(arena->offset == base, "Offset rolled back");

      // TEST 5: Resize Optimization
      TITLE("Resize Strategy");
      c_arena_reset(arena);
      (void)c_new(arena, int, 10); // Obstacle
      int *arr = c_new(arena, int, 2).unwrap;
      
      // Fast Path (In-Place)
      AllocResult rs1 = c_resize_array(arena, arr, int, 2, 4);
      REQUIRE(rs1.unwrap == arr, "Fast Path: Pointer unchanged");

      // Slow Path (Relocate)
      (void)c_new(arena, double, 1); // Bury it
      AllocResult rs2 = c_resize_array(arena, arr, int, 4, 8);
      REQUIRE(rs2.unwrap != arr, "Slow Path: Pointer moved");

      // TEST 6: Dynamic List
      TITLE("List<T>");
      c_arena_reset(arena);
      List list;
      c_list_init(arena, &list, int, 2);
      for(int i=0; i<5; i++) { 
            int* n = c_list_push(arena, &list, int); 
            *n = i * 10; 
      }
      REQUIRE(list.capacity >= 5, "List grew automatically (Cap: %zu)", list.capacity);
      REQUIRE(c_list_at(&list, int, 3) == 30, "Data verification");

      // TEST 7: Strings
      TITLE("String System");
      c_arena_reset(arena);
      Str s = c_str_fmt(arena, "HP: %d", 100);
      REQUIRE(s.len == 7, "Length correct");
      REQUIRE(c_str_eq(s, c_str("HP: 100")), "Formatting correct");

      // TEST 8: Hash Table
      TITLE("Hash Table");
      c_arena_reset(arena);
      Table map;
      c_table_init(arena, &map, 4);
      int val = 99;
      c_table_put(arena, &map, "xp", &val);
      
      int *got = c_table_get(&map, "xp");
      REQUIRE(got && *got == 99, "Retrieved value correctly");
      REQUIRE(c_table_get(&map, "missing") == NULL, "Missing key is NULL");

      // TEST 9: I/O
      TITLE("File System");
      c_arena_reset(arena);
      const char *f = "test.tmp";
      Str content = c_str("IO_TEST");
      REQUIRE(c_fs_write(f, content), "File written");
      Str read = c_fs_read(arena, f);
      REQUIRE(c_str_eq(read, content), "File read back matches");
      remove(f);

      // TEST 10: Auto-Cleanup (RAII)
      TITLE("Auto-Cleanup (RAII)");
      c_arena_reset(arena);
      size_t start = arena->offset;
      {
            c_scope(arena); // <--- Concise declaration
            c_new(arena, double, 100);
            REQUIRE(arena->offset > start, "Allocation occurred");
      }
      REQUIRE(arena->offset == start, "Auto-cleanup triggered successfully");

      c_arena_destroy(arena);
      printf("\nALL TESTS PASSED.\n");
      return 0;
}