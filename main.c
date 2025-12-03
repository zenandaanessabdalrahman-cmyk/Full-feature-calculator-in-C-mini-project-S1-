#include <stdio.h>
#include "mylib.h"

// Function to handle the basic arithmetic mode
void handle_standard_math() {
    clear(); // Clear everything
    box(stdscr, 0, 0); // Redraw the border
    mvprintw(1, 2, "Mode: Standard Arithmetic (+, -, *, /)");
    mvprintw(3, 2, "Enter calculation (e.g., 10.5 + 2.5): ");
    refresh();

    double num1, num2, result = 0.0;
    char op;
    
    // Ncurses' way to read formatted input (like scanf)
    // This requires the user to type the full expression and hit ENTER
    scanw("%lf %c %lf", &num1, &op, &num2); 

    // Use the logic functions defined in mylib.c
    switch(op) {
        case '+':
            result = add(num1, num2);
            break;
        case '-':
            result = subtract(num1, num2);
            break;
        case '*':
            result = multiply(num1, num2);
            break;
        case '/':
            result = divide(num1, num2);
            break;
        default:
            mvprintw(6, 2, "Error: Unknown operator '%c'", op);
            getch();
            return;
    }

    // Display the final result
    mvprintw(6, 2, "Result: %.2f", result);
    mvprintw(8, 2, "Press any key to return to menu...");
    getch(); // Wait for user confirmation before returning
}
int main() {
    printf("================================\n");
    printf("|||   Scalc - Core Test      |||\n");
    printf("================================\n");

    // 1. Test Basic Math
    double v1 = 10.5;
    double v2 = 2.5;
    printf("Division (%.2f / %.2f): %.2f\n", v1, v2, divide(v1, v2));

    // 2. Test Advanced Math
    printf("Sin(90): %.2f\n", my_sin(90));
    printf("Factorial(5): %lld\n", factorial(5));

    // 3. Test Complex Numbers
    printf("\n--- Complex Number Test ---\n");
    
    // Initialize two complex numbers
    // c1 = 1 + 2i
    Complex c1; 
    c1.real = 1.0; c1.imag = 2.0;

    // c2 = 3 + 4i
    Complex c2;
    c2.real = 3.0; c2.imag = 4.0;

    Complex sum = complex_add(c1, c2);
    
    printf("Adding (1+2i) + (3+4i) = ");
    print_complex(sum);

    return 0;
}