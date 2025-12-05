# 🧮 Scalc - Cross-Platform Scientific Calculator

**Mini Project 1CPS1**

**Scalc** is a powerful scientific calculator built in C. It demonstrates code portability by featuring a unique **dual-interface architecture**:
1.  **Linux TUI:** A keyboard-driven, cyberpunk-themed terminal interface using Ncurses.
2.  **Windows GUI:** A native, mouse-driven Win32 application.

Both interfaces run on the exact same logic engine (`mylib.c`), ensuring consistent precision, parsing, and error handling across platforms.

---

## 🚀 Installation & Compilation

### 🐧 Option A: Linux (TUI Version)

**Prerequisites:**
* GCC Compiler
* Ncurses Library
    * Arch Linux: `sudo pacman -S ncurses`
    * Debian/Ubuntu: `sudo apt install libncurses5-dev libncursesw5-dev`

**Build Command:**
```bash
gcc tui.c mylib.c -o scalc -lncurses -lm
