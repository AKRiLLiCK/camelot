#include "camelot/io.h"
#include <unistd.h> // write
#include <stdarg.h> // va_list
#include <string.h> // strlen

// --- 1. BASIC OUTPUT ---

void put(String s) {
      write(1, s.ptr, s.len);
}

void putn(String s) {
      put(s);
      write(1, "\n", 1);
}

// --- 2. INTEGER HELPER (PRIVATE) ---

static void put_i64(long long n) {
      if (n == 0) { write(1, "0", 1); return; }
      if (n < 0)  { write(1, "-", 1); n = -n; }

      char buf[20];
      int i = 0;
      while (n > 0) {
            buf[i++] = (n % 10) + '0';
            n /= 10;
      }
      while (i > 0) write(1, &buf[--i], 1);
}

static void put_f64(double n) {
      // 1. Handle Negative
      if (n < 0) {
            write(1, "-", 1);
            n = -n;
      }

      // 2. Split into Whole and Fraction
      long long whole = (long long)n;
      double fraction = n - whole;

      // 3. Print Whole part
      put_i64(whole);
      write(1, ".", 1);

      // 4. Print Fraction (Basic 3 decimal precision)
      // We multiply .123 by 1000 to get 123
      long long frac_int = (long long)(fraction * 1000);
      
      // Hack: Handle leading zeros (e.g. 3.001)
      if (frac_int < 100) write(1, "0", 1);
      if (frac_int < 10)  write(1, "0", 1);
      
      put_i64(frac_int);
}

// --- 3. FORMATTER ---

void print(const char *fmt, ...) {
      va_list args;
      va_start(args, fmt);

      for (const char *p = fmt; *p != '\0'; p++) {
            if (*p != '%') {
                  write(1, p, 1);
                  continue;
            }

            p++; 
            switch (*p) {
                  case 'i': // Int
                        put_i64(va_arg(args, int)); 
                        break;
                  case 'f': // Float
                        put_f64(va_arg(args, double));
                        break;
                  case 's': { // C-string
                        char *c = va_arg(args, char*);
                        write(1, c, strlen(c));
                        break;
                  }
                  case 'S': // String
                        put(va_arg(args, String)); 
                        break;
                  default: 
                        write(1, p-1, 2); 
            }
      }

      va_end(args);
}