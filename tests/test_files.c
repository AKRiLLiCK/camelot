#include "tests.h"
#include <camelot/io.h>
#include <camelot/memory.h>
#include <stdio.h> // Needed only for test setup/teardown
#include <string.h> // For strcmp/memcmp

// --- TEST HELPERS ---
static void setup_file(const char *name, const char *content) {
      FILE *f = fopen(name, "wb");
      if (f) {
            fputs(content, f);
            fclose(f);
      }
}

static void teardown_file(const char *name) {
      remove(name);
}

// --- TESTS ---

TEST(test_slurp_basic) {
      // Setup
      u8 raw_mem[1024];
      Arena a;
      hook(&a, raw_mem, 1024);

      const char *fname = "test_slurp.txt";
      const char *content = "Camelot FS";
      setup_file(fname, content);

      // Act
      String s = slurp(&a, fname);

      // Assert
      REQUIRE(s.ptr != NULL);
      REQUIRE(s.len == 10);
      REQUIRE(memcmp(s.ptr, "Camelot FS", 10) == 0);
      REQUIRE(s.ptr[10] == '\0'); // Verify the safety cap

      // Cleanup
      teardown_file(fname);
}

TEST(test_stream_dispatch) {
      const char *fname = "test_stream.txt";
      const char *content = "0123456789";
      setup_file(fname, content);

      File f = {0};

      // 1. Test OPEN
      u64 res = stream(&f, OPEN, (void*)fname, 0);
      REQUIRE(res == 1);
      REQUIRE(f.status == OK);
      REQUIRE(f.size == 10);

      // 2. Test SKIP (Skip "01234")
      u64 offset = stream(&f, SKIP, NULL, 5);
      REQUIRE(offset == 5);

      // 3. Test READ (Read "56789")
      char buf[6] = {0};
      u64 bytes = stream(&f, READ, buf, 5);
      REQUIRE(bytes == 5);
      REQUIRE(buf[0] == '5');
      REQUIRE(buf[4] == '9');

      // 4. Test CLOSE
      stream(&f, CLOSE, NULL, 0);
      REQUIRE(f.handle == NULL);
      REQUIRE(f.status != OK);

      teardown_file(fname);
}

TEST(test_missing_file) {
      u8 raw_mem[1024];
      Arena a;
      hook(&a, raw_mem, 1024);

      // Try to slurp a ghost
      String s = slurp(&a, "ghost_file.xyz");

      REQUIRE(s.ptr == NULL);
      REQUIRE(s.len == 0);
}