#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include "mylib.h"

const char *expression_ptr;
double global_last_answer = 0.0; 

void set_last_answer(double val) {
    global_last_answer = val;
}

double factorial_calc(double n) {
    if (n < 0 || n != floor(n)) return NAN; 
    if (n == 0) return 1;
    double res = 1;
    for (int i = 1; i <= (int)n; i++) {
        res *= i;
        if (isinf(res)) return res; 
    }
    return res;
}

// --- PROTOTYPES ---
double parse_expression();
double parse_term();
double parse_factor();
double parse_power();
double parse_number();

double evaluate_expression(const char *expression) {
    expression_ptr = expression;
    return parse_expression();
}

double parse_expression() {
    double result = parse_term();
    while (*expression_ptr == '+' || *expression_ptr == '-') {
        char op = *expression_ptr;
        expression_ptr++;
        if (op == '+') result += parse_term();
        else result -= parse_term();
    }
    return result;
}

// --- UPDATED: HANDLES IMPLICIT MULTIPLICATION ---
double parse_term() {
    double result = parse_factor();
    
    while (1) {
        // Skip whitespace to see what is next
        while (isspace(*expression_ptr)) expression_ptr++;

        if (*expression_ptr == '*' || *expression_ptr == '/') {
            // Explicit Multiplication/Division
            char op = *expression_ptr;
            expression_ptr++;
            if (op == '*') result *= parse_factor();
            else {
                double divisor = parse_factor();
                if (divisor != 0.0) result /= divisor;
                else result = NAN; 
            }
        }
        // Implicit Multiplication Check
        // If next char is '(', a Letter (pi/sin), or the symbol 'π' -> Multiply!
        else if (*expression_ptr == '(' || isalpha(*expression_ptr) || strncmp(expression_ptr, "π", 2) == 0) {
            result *= parse_factor();
        }
        else {
            break; // No more multiplication terms
        }
    }
    return result;
}

double parse_factor() {
    double result = parse_power();
    while (*expression_ptr == '^') {
        expression_ptr++;
        double exponent = parse_factor(); 
        result = pow(result, exponent);
    }
    return result;
}

double parse_power() {
    double result = parse_number();
    // Handle Factorial (!)
    while (*expression_ptr == '!') {
        expression_ptr++;
        result = factorial_calc(result);
    }
    return result;
}

double parse_number() {
    double result = 0.0;
    while (isspace(*expression_ptr)) expression_ptr++;

    if (*expression_ptr == '(') {
        expression_ptr++;
        result = parse_expression();
        if (*expression_ptr == ')') expression_ptr++;
        return result;
    }

    if (strncmp(expression_ptr, "π", 2) == 0) {
        expression_ptr += 2; 
        return M_PI;
    }

    if (isalpha(*expression_ptr)) {
        char name[10] = {0};
        int i = 0;
        while (isalpha(*expression_ptr) && i < 9) {
            name[i++] = *expression_ptr++;
        }
        
        if (strcmp(name, "pi") == 0) return M_PI; 
        if (strcmp(name, "e") == 0) return M_E;
        if (strcmp(name, "ANS") == 0 || strcmp(name, "ans") == 0) return global_last_answer;

        if (*expression_ptr == '(') {
            expression_ptr++;
            double val = parse_expression();
            if (*expression_ptr == ')') expression_ptr++;
            
            if (strcmp(name, "sin") == 0) return sin(val);
            if (strcmp(name, "cos") == 0) return cos(val);
            
            if (strcmp(name, "tan") == 0) {
                double t = tan(val);
                if (fabs(t) > 1e15) return NAN; 
                return t;
            }

            if (strcmp(name, "acos") == 0) return acos(val);
            if (strcmp(name, "asin") == 0) return asin(val);
            if (strcmp(name, "atan") == 0) return atan(val);
            if (strcmp(name, "sqrt") == 0) return sqrt(val);
            if (strcmp(name, "log") == 0) return log10(val);
            if (strcmp(name, "ln") == 0) return log(val);
            if (strcmp(name, "exp") == 0) return exp(val);
        }
        return 0.0;
    }

    char *end_ptr;
    result = strtod(expression_ptr, &end_ptr);
    if (expression_ptr == end_ptr) return 0.0;
    expression_ptr = end_ptr;

    return result;
}