<div align="center">

<img src="logo.svg" alt="Camelot Logo" width="180"/>

# Camelot
**The Senior Engineer's C Framework.**
A modern standard library replacement for C23 enforcing safety, ergonomics, and architectural integrity.

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)
[![Standard: C23](https://img.shields.io/badge/standard-C23-purple.svg)](https://en.wikipedia.org/wiki/C23_(C_standard_revision))
[![Documentation: GitBook](https://img.shields.io/badge/docs-GitBook-blue.svg)](https://camelot-1.gitbook.io/docs/)

[**Philosophy**](#-philosophy) • [**Quick Start**](#-quick-start) • [**Documentation**](https://camelot-1.gitbook.io/docs/)

</div>

---

## 📘 Philosophy

**"C is legacy."** We reject this statement.

C runs the world. The problem isn't the language; it is the **Standard Library**. `libc` is stuck in 1989—unsafe string handling, manual memory management hell, and inconsistent I/O.

**Camelot** bridges the gap. We bring modern concepts—Linear Allocators (Arenas), RAII Scopes, Slice-based Strings, and Generic Collections—into the raw power of C. We believe in:

1.  **Workspaces, not Malloc:** Memory is managed in scopes, not individual pointers.
2.  **Views, not Copies:** Data is immutable and shared by default.
3.  **Transparency:** No hidden allocations. No magic.

---

## ⚔️ The Manifesto

**"Why use C in the 2020s?"**

It is the industry's favorite question. The answer is not nostalgia; it is architectural necessity. C remains the foundational substrate of our digital infrastructure and the universal protocol through which all other languages must communicate.

We have compiled a comprehensive technical defense of C, dismantling the "Legacy" myth with architectural proofs and industry data.

👉 **[Read the Full Manifesto](https://camelot-1.gitbook.io/docs/manifesto)**

---

## 📚 Documentation

Detailed API references, architecture deep-dives, and performance characteristics are available on our official documentation site:

👉 **[Read the Camelot Docs](https://camelot-1.gitbook.io/docs/)**

### Core Features

* **🧠 Smart Memory:** Linear Allocators (Arenas) with `scoped` cleanup attributes. Eliminate leaks and fragmentation.
* **🧱 Strings:** Fat Pointers (`ptr` + `len`) that make buffer overflows mathematically impossible.
* **📡 I/O:** Type-safe printing and buffer-proof scanning (`io.scan`) that handles pipes and files uniformly.
* **📦 Collections:** Zero-copy Paged Lists and Linear-Probing Hash Tables.

---

## 📂 Project Structure

```text
Camelot/
├── include/       # 📢 Public Interface (What you #include)
├── src/           # ⚙️ Internal Engine (Implementation)
├── tests/         # 🛡️ Unit Test Suite
├── packages/      # 🎁 Build Artifacts (Distributables)
└── Makefile       #    Build Automation
```

---

## 🗺️ Roadmap

The kernel is complete. Phase 3 focuses on high-level systems:

* [ ] **Serialization:** Zero-allocation JSON parser (AST on Arena).
* [ ] **Concurrency:** `Thread` and `Channel` primitives (Go-style).
* [ ] **Networking:** `net` namespace for TCP/UDP servers.

---

## 📄 License

**MIT**. Build something great.
