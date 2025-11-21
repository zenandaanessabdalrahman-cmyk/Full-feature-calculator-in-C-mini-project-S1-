#include <stdio.h>
#include "mylib.h"

int add(int a, int b) {
    int result = a + b;
    return result;
}

int subtract(int a, int b) {
    int result = a - b;
    return result;
}

int divide(int a, int b) {
    if (b == 0) {
        printf("Error: Division by zero\n");
        return 0;
    }
    int result = a / b;
    return result;
}

int modulus(int a, int b) {
    if (b == 0) {
        printf("Error: Division by zero\n");
        return 0;
    }
    int result = a % b;
    return result;
}

int power(int base, int exp) {
    int result = 1;
    int i = 0;
    while (i < exp) {
        result = result * base;
        i = i + 1;
    }
    return result;
}

int factorial(int n) {
    if (n < 0) {
        printf("Error: Factorial of negative number\n");
        return -1;
    }
    int result = 1;
    int i = 1;
    while (i <= n) {
        result = result * i;
        i = i + 1;
    }
    return result;
}

int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

int lcm(int a, int b) {
    int product = a * b;
    int divisor = gcd(a, b);
    int result = product / divisor;
    return result;
}

int max(int a, int b) {
    if (a > b) {
        return a;
    } else {
        return b;
    }
}

int min(int a, int b) {
    if (a < b) {
        return a;
    } else {
        return b;
    }
}

int abs_val(int a) {
    if (a < 0) {
        return -a;
    } else {
        return a;
    }
}

int square(int a) {
    int result = a * a;
    return result;
}

int cube(int a) {
    int result = a * a * a;
    return result;
}

int sqrt_int(int a) {
    if (a < 0) {
        printf("Error: Square root of negative number\n");
        return -1;
    }
    int result = 0;
    while (result * result <= a) {
        result = result + 1;
    }
    return result - 1;
}
int exp(int base, int exp) {
    int result = 1;
    int i = 0;
    while (i < exp) {
        result = result * base;
        i = i + 1;
    }
    return result;
}
int log_int(int a, int base) {
    if (a <= 0 || base <= 1) {
        printf("Error: Invalid input for logarithm\n");
        return -1;
    }
    int result = 0;
    int power = 1;
    while (power <= a) {
        power = power * base;
        result = result + 1;
    }
    return result - 1;
}
float sin_int(int degrees) {
    float radians = degrees * 3.14159 / 180.0;
    float term = radians;
    float sum = term;
    int n = 1;
    while (n < 10) {
        term = -term * radians * radians / ((2 * n) * (2 * n + 1));
        sum = sum + term;
        n = n + 1;
    }
    return sum;
}
float cos_int(int degrees) {
    float radians = degrees * 3.14159 / 180.0;
    float term = 1.0;
    float sum = term;
    int n = 1;
    while (n < 10) {
        term = -term * radians * radians / ((2 * n - 1) * (2 * n));
        sum = sum + term;
        n = n + 1;
    }
    return sum;
}
float tan_int(int degrees) {
    float sine = sin_int(degrees);
    float cosine = cos_int(degrees);
    if (cosine == 0) {
        printf("Error: Tangent undefined for this angle\n");
        return 0;
    }
    return sine / cosine;
}
float loge_int(int a) {
    if (a <= 0) {
        printf("Error: Invalid input for natural logarithm\n");
        return -1;
    }
    float result = 0.0;
    float term = (a - 1);
    int n = 1;
    while (n < 100) {
        if (n % 2 == 1) {
            result = result + term / n;
        } else {
            result = result - term / n;
        }
        term = term * (a - 1);
        n = n + 1;
    }
    return result;
}
float exp_float(float x) {
    float result = 1.0;
    float term = 1.0;
    int n = 1;
    while (n < 100) {
        term = term * x / n;
        result = result + term;
        n = n + 1;
    }
    return result;
}
float sqrt_float(float a) {
    if (a < 0) {
        printf("Error: Square root of negative number\n");
        return -1;
    }
    float guess = a / 2.0;
    float epsilon = 0.00001;
    while (1) {
        float next_guess = (guess + a / guess) / 2.0;
        if (abs_val(next_guess - guess) < epsilon) {
            break;
        }
        guess = next_guess;
    }
    return guess;
}
float power_float(float base, int exp) {
    float result = 1.0;
    int i = 0;
    while (i < exp) {
        result = result * base;
        i = i + 1;
    }
    return result;
}
float divide_float(float a, float b) {
    if (b == 0.0) {
        printf("Error: Division by zero\n");
        return 0.0;
    }
    float result = a / b;
    return result;
}
float modulus_float(float a, float b) {
    if (b == 0.0) {
        printf("Error: Division by zero\n");
        return 0.0;
    }
    float result = a - ((int)(a / b)) * b;
    return result;
}
