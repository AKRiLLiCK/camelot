<div align="center">
  <img src="logo.png" alt="Camelot Logo" width="180" />
  <h1>Camelot Framework</h1>
  <p><strong>Make C Safe Again.</strong></p>
  
  <p>
    <a href="#-philosophy">Philosophy</a> •
    <a href="#-features">Features</a> •
    <a href="#-quick-start">Quick Start</a> •
    <a href="#-credits">Credits</a>
  </p>
</div>

---

## 🏰 Philosophy

**University teaches us to survive in the wilderness.** We are taught to manually manage memory with `malloc` and `free`, parse raw text with `scanf`, and juggle `char*` pointers without safety nets. This leads to memory leaks, buffer overflows, and undefined behavior.

**Camelot builds a fortress.**
This framework enforces modern engineering practices on top of C11. It "poisons" legacy unsafe functions at the compiler level, forcing the developer to use safer, region-based memory management and sanitized I/O.

## 🛡️ Features

### 1. The Poison Barrier 🚫
Camelot uses GCC preprocessor directives to **ban** standard unsafe functions. If you try to use them, your code will not compile.
* **Banned:** `malloc`, `free`, `realloc`, `calloc`
* **Banned:** `scanf`, `gets`, `strcpy`, `strcat`

### 2. Linear Memory Arenas (The Engine) 🧠
Forget fragmentation and memory leaks. Camelot uses **Region-Based Memory Management**.
* **O(1) Allocation:** Memory is allocated by bumping a pointer. No syscalls per object.
* **O(1) Deallocation:** You don't free variables. You wipe the entire Arena instantly.
* **Scoped Safety:** Supports `__attribute__((cleanup))` for automatic scope-based resource management.

### 3. Sanitized I/O 📝
Input processing that doesn't break your program.
* **`c_read()`**: Handles buffering, parsing, and validation internally. No more infinite loops from `scanf`.
* **`String`**: A proper struct with a pointer and length. No more missing null-terminator bugs.

---

## 🚀 Quick Start

### Prerequisites
* GCC or Clang
* Make

### Installation
Clone the repository:
```bash
git clone [https://github.com/yourusername/camelot.git](https://github.com/yourusername/camelot.git)
cd camelot
````

-----

## 📂 Project Structure

```text
Camelot/
├── Makefile                # Build automation
├── include/
│   └── camelot.h           # The Public Interface (and Poison)
├── src/
│   ├── camelot_memory.c    # Linear Arena implementation
│   ├── camelot_io.c        # Input Sanitizer implementation
│   └── camelot_string.c    # String type logic
└── main.c                  # Showcase Application
```

-----

## 🎨 Credits

**Author:** Acrilic  
**License:** MIT

### Assets
* Logo by Solarus
