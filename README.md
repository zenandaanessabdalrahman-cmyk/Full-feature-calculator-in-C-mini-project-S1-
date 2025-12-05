Here is the raw source code for your **README.md**. You can copy this entire block and paste it directly into the file on GitHub.

````markdown
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
````

**Run:**

```bash
./scalc
```

### 🪟 Option B: Windows (Native GUI Version)

**Prerequisites:**

  * MinGW GCC Compiler (on Windows) OR
  * MinGW-w64 (on Linux for cross-compilation).

**Build Command (Cross-Compile from Linux):**

```bash
x86_64-w64-mingw32-gcc win_gui.c mylib.c -o scalc.exe -mwindows -lm
```

**Build Command (On Windows via MinGW/Git Bash):**

```bash
gcc win_gui.c mylib.c -o scalc.exe -mwindows
```

**Run:**

  * Double-click `scalc.exe` to launch the native window.

-----

## 🎮 Controls & Interface

### Linux TUI Mode

  * **Navigation:** Use **Arrow Keys** to move selection.
  * **Select:** Press `ENTER` to click a button.
  * **Typing:** Type numbers and functions directly on the keyboard.
  * **Theme:** Cyberpunk (Cyan text on Black background).

### Windows GUI Mode

  * **Navigation:** Use the **Mouse** to click buttons.
  * **Typing:** Physical keyboard input is fully supported.
  * **Visuals:** Uses standard Windows native controls and system fonts.

-----

## 🌟 Key Features (Both Platforms)

### 🧠 Smart Parsing (Implicit Multiplication)

Scalc understands math the way you write it.

  * `2pi` → interpreted as `2 * π`
  * `5(2+2)` → interpreted as `5 * (2+2)`
  * `cos(2pi)` → interpreted as `cos(2 * π)`

### 👁️ Intelligent Output

The calculator displays results in the most human-readable format possible.

  * **Pi Detection:** Results close to Pi display as **π** or **-π** (e.g., `acos(-1)` → `π`).
  * **Pretty Print:** Large numbers are formatted cleanly (e.g., 1 Quadrillion is `1 x 10^15` instead of `1e+15`).
  * **Mantissa Cleanup:** `5.0000 x 10^-6` becomes `5 x 10^-6`.

### 💾 Stateful Memory (ANS)

  * Calculations chain automatically.
  * Type `5 + 5` (=10), then press `+ 2`. The calculator assumes `10 + 2`.
  * **ANS Button:** Manually recall the last result.

-----

## 📚 Supported Functions

| Category | Functions | Example |
| :--- | :--- | :--- |
| **Basic** | `+`, `-`, `*`, `/`, `^` | `2^3 = 8` |
| **Factorial** | `!` | `5!` = 120 |
| **Trig** | `sin`, `cos`, `tan` | `sin(π/2) = 1` |
| **Inverse Trig** | `asin`, `acos`, `atan` | `acos(-1) = π` |
| **Logs** | `log` (Base 10), `ln` (e) | `log(100) = 2` |
| **Roots** | `sqrt` | `sqrt(25) = 5` |
| **Constants** | `π` (pi), `e`, `ANS` | `cos(2π)` |

-----

## ⚠️ Error Handling

Scalc prevents crashes by catching mathematical errors before they happen. **"Math Error"** appears for:

  * Division by zero.
  * Square root of negative numbers.
  * Trig domain errors (e.g., `acos(8)`).
  * Values exceeding **1 Quadrillion** ($10^{15}$).

-----

## 👨‍💻 Project Architecture

This project demonstrates code portability by separating Logic from Interface.

  * **`mylib.c` (The Brain):** Contains the Recursive Descent Parser, math logic, and error handling. This file is **identical** for both Windows and Linux.
  * **`tui.c` (The Linux Body):** Uses Ncurses to draw the terminal interface.
  * **`win_gui.c` (The Windows Body):** Uses Win32 API to create native windows and buttons.

<!-- end list -->
