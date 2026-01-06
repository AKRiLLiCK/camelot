# --- CAMELOT BUILD SYSTEM ---

CC      = gcc
AR      = ar
CFLAGS  = -I include -Wall -Wextra -std=c2x -Wno-unused-function

# 1. Source Directories
SRCS    = $(wildcard src/memory/*.c) \
          $(wildcard src/types/*.c) \
          $(wildcard src/io/*.c) \
          $(wildcard src/ds/*.c)

# 2. Test Source
TEST_SRCS = $(wildcard tests/*.c)

# 3. Object Files
OBJS    = $(SRCS:.c=.o)

# 4. Output Configuration
OUT_DIR = build
TARGET  = $(OUT_DIR)/test_runner
LIB     = $(OUT_DIR)/libcamelot.a

# --- RULES ---

.PHONY: all clean test dirs

all: $(LIB)

dirs:
	@mkdir -p $(OUT_DIR)

# --- TEST SUITE ---
# If ./test_runner returns 1, Make will abort with "Error 1", failing the CI.
test: dirs
	@echo " [CC]   Compiling Test Suite..."
	@$(CC) $(CFLAGS) $(TEST_SRCS) $(SRCS) -o $(TARGET)
	@echo " [EXEC] Running Tests..."
	@./$(TARGET)

# --- LIBRARY GENERATION ---

%.o: %.c
	@echo " [CC]   $<"
	@$(CC) $(CFLAGS) -c $< -o $@

$(LIB): dirs $(OBJS)
	@echo " [AR]   Creating Static Library..."
	@$(AR) rcs $(LIB) $(OBJS)
	@rm -f $(OBJS)

clean:
	@echo " [RM]   Cleaning artifacts..."
	@rm -rf $(OUT_DIR)
	@rm -f src/*/*.o