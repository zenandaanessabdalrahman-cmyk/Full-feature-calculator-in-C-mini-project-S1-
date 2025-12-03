
# 🧮 Scalc - Full Feature Calculator in C

**Mini Project 1CPS1**

**Scalc** is a powerful, terminal-based scientific calculator built in C. It combines the visual accessibility of a GUI with the speed of a command-line tool, featuring a cyberpunk aesthetic, smart parsing, and a responsive interface.

## 🚀 Installation

### Prerequisites
You need a C compiler (`gcc`) and the Ncurses library.

* **Arch Linux:**
    ```bash
    sudo pacman -S ncurses
    ```
* **Debian/Ubuntu:**
    ```bash
    sudo apt install libncurses5-dev libncursesw5-dev
    ```

### Compilation
Run the following command in the terminal to build the calculator:
```bash
gcc tui.c mylib.c -o scalc -lncurses -lm
````

### Running

```bash
./scalc
```

-----

## 🎮 Controls

Scalc supports two modes of interaction simultaneously: **Mouse-driven** (via arrows) and **Keyboard-driven**.

| Key | Action |
| :--- | :--- |
| **Arrow Keys** | Navigate the button grid (Up, Down, Left, Right). |
| **ENTER** | Select the highlighted button. |
| **Keyboard** | Type numbers and functions directly (`sin`, `+`, `5`, etc.). |
| **=** | Calculate the result. |
| **Backspace** | Delete the last character. |
| **Q** or **ESC** | Quit the application. |

-----

## 🌟 Key Features

### 🧠 Smart Parsing (Implicit Multiplication)

Scalc understands math the way you write it on paper. You don't always need the multiplication sign (`*`).

  * `2pi` → interpreted as `2 * π`
  * `5(2+2)` → interpreted as `5 * (2+2)`
  * `3sin(90)` → interpreted as `3 * sin(90)`

### 👁️ Intelligent Output

The calculator displays results in the most human-readable format possible.

  * **Pi Detection:** If a result is `3.14159...`, it displays **π**.
  * **Pretty Print:** Huge numbers like `10000000000` are formatted as `1 x 10^10` instead of `1e+10`.

### 💾 Stateful Memory (ANS)

Just like a physical scientific calculator, Scalc remembers your last result.

1.  Type `5 + 5` → Result: `10`
2.  Press `+ 2` → Screen shows `10+2` (Uses previous result).

<!-- end list -->

  * **ANS Button:** You can manually recall the last answer by typing `ANS` or selecting the button.

-----

## 📚 Supported Functions

| Category | Functions | Example |
| :--- | :--- | :--- |
| **Basic** | `+`, `-`, `*`, `/`, `^` (Power) | `2^3 = 8` |
| **Factorial** | `!` | `5!` = 120 |
| **Trigonometry** | `sin`, `cos`, `tan` | `sin(π/2) = 1` |
| **Inverse Trig** | `asin`, `acos`, `atan` | `acos(-1) = π` |
| **Logarithms** | `log` (Base 10), `ln` (Base e) | `log(100) = 2` |
| **Roots** | `sqrt` | `sqrt(25) = 5` |
| **Exponential** | `exp` (e^x) | `exp(1) = 2.718...` |
| **Constants** | `π` (pi), `e`, `ANS` | `cos(2π)` |

-----

## ⚠️ Error Handling

Scalc prevents crashes by catching mathematical errors before they happen. **"Math Error"** appears if you attempt:

  * Division by zero (`5/0`).
  * Square root of a negative number (`sqrt(-1)`).
  * Out of domain Trigonometry (`acos(8)`).
  * Calculations resulting in Infinity or numbers exceeding 1 Quadrillion.

-----

## 🎨 Visuals

  * **Theme:** Cyberpunk (Cyan Text on Black Background).
  * **Grid:** 8 Rows x 4 Columns, centered text with a responsive layout.
  * **UTF-8:** Native support for mathematical symbols like **π**.

-----

## 👨‍💻 Developing the Project

If you want to modify the code:

  * **`mylib.c`**: Contains the "Brain" (Parsing logic, math functions, recursive descent algorithm).
  * **`tui.c`**: Contains the "Body" (Ncurses drawing, key handling, colors).
  * **`mylib.h`**: The bridge between the Brain and the Body.

<!-- end list -->

```
```
