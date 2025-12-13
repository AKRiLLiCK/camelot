// tests.c
// The Official Verification Suite for Camelot
// Compile with: gcc tests.c src/*.c -std=c23 -o run_tests

#include "include/camelot.h"

int main() {
      printf("\n🏰 CAMELOT FORTRESS VERIFICATION 🏰\n");
      printf("---------------------------------------\n");

      // 1. Initialize Arena (1024 bytes total)
      ArenaResult sys = c_arena_create(1024);
      if (sys.error != C_OK) {
            printf("System Init Failed!\n");
            return 1;
      }
      Arena *arena = sys.unwrap;
      printf("[*] Arena Online: 1024 bytes capacity.\n");

      // --- TEST 1: Valid Allocation ---
      printf("\n[TEST 1] Allocating 2 integers...\n");
      AllocResult res1 = c_new(arena, int, 2);

      if (res1.error == C_OK) {
            int *nums = (int *)res1.unwrap;
            nums[0] = 42;
            nums[1] = 99;
            printf("      [PASS] Allocation Success. Data: [%d, %d]\n", nums[0], nums[1]);
      } else {
            printf("      [FAIL] Unexpected allocation failure!\n");
      }

      // --- TEST 2: Intentional OOM ---
      printf("\n[TEST 2] Attempting to allocate 10,000 bytes (Should Fail)...\n");
      AllocResult res2 = c_new(arena, char, 10000);

      if (res2.error == C_OOM) {
            printf("      [PASS] System correctly blocked the allocation (Error: C_OOM)\n");
      } else {
            printf("      [FAIL] System allowed the allocation or gave wrong error!\n");
      }

      // --- TEST 3: The Reset Superpower ---
      printf("\n[TEST 3] Testing Arena Reset (Reuse)...\n");

      AllocResult fill_res = c_new(arena, int, 100); 
      if (fill_res.error == C_OK) {
            printf("      [INFO] Used before reset: %zu bytes\n", arena->offset);
      } else {
            printf("      [FAIL] Setup failed (OOM during fill).\n");
            return 1;
      }

      c_arena_reset(arena);
      printf("      [ACTION] Arena Reset Triggered.\n");

      if (arena->offset == 0) {
            printf("      [PASS] Offset is 0. Memory is ready for reuse.\n");
      } else {
            printf("      [FAIL] Offset is not 0!\n");
      }

      AllocResult res3 = c_new(arena, int, 1);
      if (res3.error == C_OK) {
            printf("      [PASS] Re-allocation successful at start of buffer (%p)\n", res3.unwrap);
            if (res3.unwrap == (void*)((uintptr_t)arena->buffer)) {
                  printf("      [PASS] Pointer address matches buffer start.\n");
            }
      }

      // --- TEST 4: Temporary Scopes (Time Travel) ---
      printf("\n[TEST 4] Testing Temporary Scopes...\n");
      c_arena_reset(arena);

      AllocResult p_res = c_new(arena, int, 10); // 40 bytes
      size_t permanent_offset = arena->offset;
      printf("      [INFO] Base Offset: %zu bytes\n", permanent_offset);

      {
            TempArena scratch = c_temp_begin(arena);
            c_new(arena, double, 100); // 800 bytes
            printf("      [INFO] Offset inside scope: %zu bytes\n", arena->offset);

            if (arena->offset <= permanent_offset) {
                  printf("      [FAIL] Pointer didn't move inside scope!\n");
            }
            c_temp_end(scratch);
      }

      printf("      [INFO] Offset after scope: %zu bytes\n", arena->offset);
      if (arena->offset == permanent_offset) {
            printf("      [PASS] Time Travel Successful. Memory rolled back perfectly.\n");
      } else {
            printf("      [FAIL] Memory leak! Offset is %zu, expected %zu\n", arena->offset, permanent_offset);
      }

      // --- TEST 5: The Resize Optimization ---
      printf("\n[TEST 5] Testing Resize Optimization...\n");
      c_arena_reset(arena);

      (void)c_new(arena, int, 10); // Obstacle

      AllocResult arr_res = c_new(arena, int, 2);
      int *arr = (int *)arr_res.unwrap;
      arr[0] = 100; 
      arr[1] = 200;
      printf("      [INFO] Original Array: %p (Values: %d, %d)\n", (void*)arr, arr[0], arr[1]);

      printf("      [ACTION] Expanding array in-place (Fast Path)...\n");
      AllocResult resize_1 = c_resize_array(arena, arr, int, 2, 4);
      
      if (resize_1.unwrap == arr) {
            printf("      [PASS] Pointer address stayed the same! (Zero Copy)\n");
      } else {
            printf("      [FAIL] Pointer moved! Optimization failed.\n");
      }

      (void)c_new(arena, double, 1); // Bury the array
      printf("      [ACTION] Allocated new object. Array is now 'buried'.\n");

      printf("      [ACTION] Expanding buried array (Slow Path)...\n");
      AllocResult resize_2 = c_resize_array(arena, arr, int, 4, 8);
      
      if (resize_2.unwrap != arr) {
            printf("      [PASS] Pointer moved to %p (Data preserved)\n", resize_2.unwrap);
            int *new_arr = (int *)resize_2.unwrap;
            if (new_arr[0] == 100 && new_arr[1] == 200) {
                  printf("      [PASS] Data copied correctly.\n");
            } else {
                  printf("      [FAIL] Data corruption detected!\n");
            }
      } else {
            printf("      [FAIL] Pointer didn't move! We overwrote the next object!\n");
      }

      // --- TEST 6: Dynamic List (Vectors) ---
      printf("\n[TEST 6] Testing Dynamic List (Vectors)...\n");
      c_arena_reset(arena);

      typedef struct {
            int id;
            float x, y;
      } Entity;

      List entities;
      c_list_init(arena, &entities, Entity, 2); 
      printf("      [INFO] Init Capacity: %zu\n", entities.capacity);

      printf("      [ACTION] Spawning 5 entities...\n");
      for (int i = 0; i < 5; i++) {
            Entity *e = c_list_push(arena, &entities, Entity);
            if (e) {
                  e->id = i;
                  e->x = i * 10.0f;
                  e->y = i * -5.0f;
            }
      }

      if (entities.capacity >= 5) {
            printf("      [PASS] List grew automatically. Cap: %zu\n", entities.capacity);
      } else {
            printf("      [FAIL] List did not grow!\n");
      }
      printf("      [DATA] Entity 3 ID: %d (Expected 3)\n", c_list_at(&entities, Entity, 3).id);

      // --- TEST 7: Safe Strings ---
      printf("\n[TEST 7] Testing String System...\n");
      c_arena_reset(arena);

      Str s1 = c_str("Hello");
      printf("      [INFO] Wrapped String: '" STR_FMT "' (Len: %zu)\n", STR_ARG(s1), s1.len);

      int hp = 100;
      const char *name = "PlayerOne";
      Str status = c_str_fmt(arena, "[User: %s | HP: %d]", name, hp);
      printf("      [INFO] Formatted: '" STR_FMT "'\n", STR_ARG(status));

      Str s2 = c_str("Hello");
      if (c_str_eq(s1, s2)) {
            printf("      [PASS] Equality check works.\n");
      } else {
            printf("      [FAIL] Equality check failed!\n");
      }

      uintptr_t ptr_addr = (uintptr_t)status.ptr;
      uintptr_t arena_start = (uintptr_t)arena->buffer;
      uintptr_t arena_end = arena_start + arena->length;

      if (ptr_addr >= arena_start && ptr_addr < arena_end) {
            printf("      [PASS] String is safely stored inside the Fortress.\n");
      } else {
            printf("      [FAIL] String leaked to outside memory!\n");
      }

      // --- TEST 8: Hash Table ---
      printf("\n[TEST 8] Testing Hash Table (Assets)...\n");
      c_arena_reset(arena);

      Table assets;
      c_table_init(arena, &assets, 4); 
      printf("      [INFO] Table Init Capacity: %zu\n", assets.capacity);

      int tex_id_1 = 101;
      int tex_id_2 = 202;
      int tex_id_3 = 303;

      c_table_put(arena, &assets, "hero.png", &tex_id_1);
      c_table_put(arena, &assets, "enemy.png", &tex_id_2);
      c_table_put(arena, &assets, "ground.png", &tex_id_3);
      printf("      [ACTION] Inserted 3 assets.\n");

      int *val = (int *)c_table_get(&assets, "enemy.png");
      if (val && *val == 202) {
            printf("      [PASS] Found 'enemy.png' -> ID %d\n", *val);
      } else {
            printf("      [FAIL] Lookup failed or wrong value!\n");
      }

      if (c_table_get(&assets, "missing.png") == NULL) {
            printf("      [PASS] Correctly returned NULL for missing key.\n");
      }

      int new_id = 999;
      c_table_put(arena, &assets, "enemy.png", &new_id);
      val = (int *)c_table_get(&assets, "enemy.png");
      if (val && *val == 999) {
            printf("      [PASS] Value updated correctly.\n");
      }

      // --- TEST 9: File I/O ---
      printf("\n[TEST 9] Testing File System...\n");
      c_arena_reset(arena);

      const char *path = "camelot_test.txt";
      Str content = c_str("Camelot I/O System Online.");

      if (c_fs_write(path, content)) {
            printf("      [PASS] Wrote to '%s'\n", path);
      } else {
            printf("      [FAIL] Could not write file!\n");
      }

      Str read_back = c_fs_read(arena, path);
      if (c_str_eq(content, read_back)) {
            printf("      [PASS] Read back matches: '" STR_FMT "'\n", STR_ARG(read_back));
      } else {
            printf("      [FAIL] Content mismatch!\n");
      }

      remove(path);
      c_arena_destroy(arena);
      
      printf("\n---------------------------------------\n");
      printf("✅ ALL SYSTEMS OPERATIONAL.\n");
      
      return 0;
}