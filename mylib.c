#include <stdio.h>
#include "mylib.h"

int add(int a, int b) {
    return a + b;
}

int subtract(int a, int b) {
    return a - b;
}

int divide(int a, int b) {
    if (b == 0) {
        printf("Error: Division by zero\n");
        return 0;
    }
    return a / b;
}

int modulus(int a, int b) {
    if (b == 0) {
        printf("Error: Division by zero\n");
        return 0;
    }
    return a % b;
}

int pow_int(int base, int exp) {
    if (exp < 0) {
        printf("Error: Negative exponent not supported in pow_int\n");
        return 0;
    }
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
    a = abs_val(a);
    b = abs_val(b);
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

int lcm(int a, int b) {
    if (a == 0 || b == 0) return 0;
    int divisor = gcd(a, b);
    return (a / divisor) * b;  
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

int min(int a, int b) {
    return (a < b) ? a : b;
}

int abs_val(int a) {
    return (a < 0) ? -a : a;
}

int square(int a) {
    return a * a;
}

int cube(int a) {
    return a * a * a;
}

int sqrt_int(int a) {
    if (a < 0) {
        printf("Error: Square root of negative number\n");
        return -1;
    }
    int result = 0;
    while (result * result <= a) {
        result++;
    }
    return result - 1;
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
        result++;
    }
    return result - 1;
}

float fabs_val(float x) {
    return (x < 0.0f) ? -x : x;
}

#define PI 3.14159265358979323846f

float sin_int(int degrees) {
    float radians = degrees * PI / 180.0f;
    float term = radians;
    float sum = term;
    int n = 1;
    while (n < 10) {
        term = -term * radians * radians / ((2 * n) * (2 * n + 1));
        sum += term;
        n++;
    }
    return sum;
}

float cos_int(int degrees) {
    float radians = degrees * PI / 180.0f;
    float term = 1.0f;
    float sum = term;
    int n = 1;
    while (n < 10) {
        term = -term * radians * radians / ((2 * n - 1) * (2 * n));
        sum += term;
        n++;
    }
    return sum;
}

float tan_int(int degrees) {
    float cosine = cos_int(degrees);
    if (fabs_val(cosine) < 0.0001f) {
        printf("Error: Tangent undefined for this angle\n");
        return 0.0f;
    }
    return sin_int(degrees) / cosine;
}

float loge_approx(float a) {
    if (a <= 0.0f) {
        printf("Error: Natural log undefined for non-positive input\n");
        return -1.0f;
    }
    float x = a - 1.0f;
    if (x <= -1.0f || x >= 1.0f) {
        printf("Warning: loge_approx is inaccurate for this input\n");
    }
    float result = 0.0f;
    float term = x;
    int n = 1;
    while (n < 50) {
        if (n % 2 == 1)
            result += term / n;
        else
            result -= term / n;
        term *= x;
        n++;
    }
    return result;
}

float exp_float(float x) {
    float result = 1.0f;
    float term = 1.0f;
    int n = 1;
    while (n < 100) {
        term = term * x / n;
        result += term;
        n++;
    }
    return result;
}

float sqrt_float(float a) {
    if (a < 0.0f) {
        printf("Error: Square root of negative number\n");
        return -1.0f;
    }
    if (a == 0.0f) return 0.0f;
    float guess = a / 2.0f;
    float epsilon = 0.00001f;
    while (1) {
        float next_guess = (guess + a / guess) / 2.0f;
        if (fabs_val(next_guess - guess) < epsilon) {
            return next_guess;
        }
        guess = next_guess;
    }
}

float pow_float(float base, int exp) {
    if (exp < 0) {
        printf("Error: Negative exponent not supported in pow_float\n");
        return 0.0f;
    }
    float result = 1.0f;
    int i = 0;
    while (i < exp) {
        result *= base;
        i++;
    }
    return result;
}

float divide_float(float a, float b) {
    if (fabs_val(b) < 0.00001f) {
        printf("Error: Division by zero\n");
        return 0.0f;
    }
    return a / b;
}

float modulus_float(float a, float b) {
    if (fabs_val(b) < 0.00001f) {
        printf("Error: Division by zero\n");
        return 0.0f;
    }
   
    float aa = fabs_val(a);
    float bb = fabs_val(b);
    while (aa >= bb) {
        aa -= bb;
    }
    return (a < 0) ? -aa : aa;
}


float degrees_to_radians(float deg) {
    return deg * 3.14159f / 180.0f;
}

float radians_to_degrees(float rad) {
    return rad * 180.0f / 3.14159f;
}


int cbrt_int(int a) {
    if (a == 0) return 0;
    int negative = 0;
    if (a < 0) {
        negative = 1;
        a = -a;
    }
    int guess = 1;
    while (guess * guess * guess <= a) {
        guess = guess + 1;
    }
    guess = guess - 1;
    if (negative) {
        return -guess;
    } else {
        return guess;
    }
}

float cbrt_float(float a) {
    if (a == 0.0f) return 0.0f;
    int negative = 0;
    if (a < 0.0f) {
        negative = 1;
        a = -a;
    }
    float guess = a / 3.0f;
    float epsilon = 0.00001f;
    while (1) {
        float cube = guess * guess * guess;
        if (cube > a) {
            guess = guess - (cube - a) / (3 * guess * guess + 0.0001f);
        } else {
            guess = guess + (a - cube) / (3 * guess * guess + 0.0001f);
        }
        if (guess * guess * guess > a - epsilon && guess * guess * guess < a + epsilon) {
            break;
        }
    }
    if (negative) {
        return -guess;
    } else {
        return guess;
    }
}

int is_prime(int n) {
    if (n < 2) {
        return 0;
    }
    if (n == 2) {
        return 1;
    }
    if (n % 2 == 0) {
        return 0;  
    }
    int i = 3;
    while (i * i <= n) {
        if (n % i == 0) {
            return 0;
        }
        i = i + 2;
    }
    return 1;
}