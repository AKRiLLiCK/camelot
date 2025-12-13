CC = gcc
# We use -Iinclude so we can do <camelot.h> if we wanted, 
# but currently your code uses "include/camelot.h" which is fine.
CFLAGS = -std=c2x -Wall -Wextra -Wno-unused-result -Iinclude

# Define all source files
SRCS = tests.c \
       src/camelot_memory.c \
       src/camelot_list.c \
       src/camelot_string.c \
       src/camelot_table.c \
       src/camelot_io.c

# The output executable name
TARGET = run_tests

# --- TARGETS ---

# Default: Build the test suite
all: $(TARGET)

# Link everything together
$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)

# Build AND Run (Shortcut for development)
test: $(TARGET)
	@echo "--- RUNNING TESTS ---"
	@./$(TARGET)

# Clean up artifacts
clean:
	rm -f $(TARGET) $(TARGET).exe