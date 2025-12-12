<div align="center">

<img src="logo.png" alt="Camelot Logo" width="200"/>

# Camelot
Re-Imagining C for modern engineering. A framework enforcing safety, ergonomics, and structural integrity for the C language.

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Standard: C23](https://img.shields.io/badge/standard-C23-purple.svg)](https://en.wikipedia.org/wiki/C23_(C_standard_revision))
[![Build: Make](https://img.shields.io/badge/build-makefile-green.svg)](Makefile)

[**Philosophy**](#-philosophy) • [**Features**](#-features) • [**Quick Start**](#-getting-started) • [**Internals**](#-project-structure)

</div>

---

## 📘 Philosophy

C is often dismissed as "legacy"—a dangerous relic destined to be replaced. **This view ignores reality.**

The world's most critical infrastructure—from the **Linux Kernel** to **Git**, **Redis**, and **SQLite**—is built on C. It remains the gold standard for portability and stability. Experienced engineers don't hate C; they love it for its **honesty**. In C, there is no hidden garbage collector, no massive runtime, and no "magic" slowing you down. It is surprisingly ergonomic when stripped of its historical baggage.

**The "pain" of C comes from the archaic patterns taught in academia.**
Universities train us to manually `free()` every single allocation and wrestle with brittle `scanf` parsing. That isn't "systems programming"—that is just bad engineering. Real-world systems programmers use **Memory Arenas** to handle allocations instantly and **Data-Oriented patterns** to manage state.

**Camelot brings this "Senior Engineer" workflow to the forefront.**
We strip away the friction of the 1989 standard library to reveal the high-performance, expressive language underneath.

**The Goal:** Prove that C is the ultimate tool for modern engineering.
- ❌ **Banned:** No manual `free()` drudgery. No `scanf` traps.
- ✅ **Ergonomic:** Memory handling is O(1) and linear via Arenas (just like modern game engines).
- ✅ **Transparent:** You control the machine, but the machine doesn't fight you.

---

## 🛡️ Features

Camelot enforces modern engineering practices through a combination of compile-time restrictions and runtime helpers.

| Feature | Description | Status |
| :--- | :--- | :--- |
| **Poison Barrier** 🚫 | Blocks `malloc`, `free`, `scanf`, `gets`, `strcpy`. | 🟡 WIP |
| **Linear Arena** 🧠 | Region-based memory. Allocations are instant pointers bumps. | 🟡 WIP |
| **Auto-Cleanup** 🧹 | Uses `__attribute__((cleanup))` for scoped deallocation. | 🟡 WIP |
| **Input Sanitizer** 📝 | Replaces `scanf` with buffered, type-safe readers. | 🟡 WIP |

**Status:** 🟢 **Active** (Stable) • 🟡 **WIP** (Construction) • 🔴 **Planned**

---

## 🚀 Getting Started

### Prerequisites
* GCC or Clang
* Make

### Workflow

1. **Clone the repo:**
   ```bash
   git clone https://github.com/AKRiLLiCK/camelot
   cd camelot
   ```

2.  **Build the framework:**

    ```bash
    make
    ```

3.  **Start Coding:**
    Link your `main.c` against the Camelot object files. If you try to use banned functions like `malloc` or `scanf`, the compiler will throw an error and stop the build.

-----

## 📂 Project Structure

A clean, ISO-standard C structure designed for portability.

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

## 📄 License

MIT — Use, fork, learn, experiment.  
See [LICENSE](https://www.google.com/search?q=LICENSE) for details.

## 🎨 Credits

**Author:** Acrilic

### Assets

  * Logo made by [Solarus](https://www.flaticon.com/free-icon/cube_17533083)
