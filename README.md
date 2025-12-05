# 🧮 Scalc - Cross-Platform Scientific Calculator

**Mini Project 1CPS1**

**Scalc** is a powerful scientific calculator built in C. It features a unique **dual-interface architecture**:
1.  **Linux TUI:** A keyboard-driven, cyberpunk-themed terminal interface.
2.  **Windows GUI:** A native, mouse-driven Win32 application.

Both interfaces run on the exact same logic engine (`mylib.c`), ensuring consistent precision across platforms.

---

## 🚀 Installation & Compilation

### 🐧 Option A: Linux (TUI Version)

**Prerequisites:**
* GCC Compiler
* Ncurses Library (`sudo pacman -S ncurses` or `sudo apt install libncurses5-dev`)

**Build Command:**
```bash
gcc tui.c mylib.c -o scalc -lncurses -lm
