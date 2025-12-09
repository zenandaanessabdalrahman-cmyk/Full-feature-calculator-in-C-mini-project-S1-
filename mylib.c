#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "mylib.h"

const char *expression_ptr;
Complex global_last_answer = {0.0, 0.0}; 
int angle_mode = 0; 
int fraction_mode = 0; 

char global_error_msg[256] = {0};

void set_error(const char *msg) {
    if (strlen(global_error_msg) == 0) strcpy(global_error_msg, msg);
}

const char* get_global_error() { return global_error_msg; }
void clear_global_error() { global_error_msg[0] = '\0'; }

void set_last_answer(Complex val) { global_last_answer = val; }
void toggle_angle_mode() { angle_mode = !angle_mode; }
int get_angle_mode() { return angle_mode; }

void toggle_fraction_mode() { fraction_mode = !fraction_mode; }
int get_fraction_mode() { return fraction_mode; }

double my_fabs(double x) { return (x < 0) ? -x : x; }

int to_fraction(double val, long *num, long *den) {
    if ((long)val == val) return 0; 
    
    double tolerance = 1.0e-6;
    double h1=1, h2=0, k1=0, k2=1;
    double b = val;
    
    for(int i=0; i<20; i++) {
        long a = (long)b;
        long aux = h1; h1 = a * h1 + h2; h2 = aux;
        aux = k1; k1 = a * k1 + k2; k2 = aux;
        
        double approx = (double)h1 / k1;
        if (my_fabs(val - approx) < tolerance) {
            *num = h1; *den = k1; return 1;
        }
        if (k1 > 10000) break; 
        b = 1.0 / (b - a);
    }
    return 0; 
}

double to_rad(double angle) { return angle * (MY_PI / 180.0); }
double to_deg(double angle) { return angle * (180.0 / MY_PI); }

Complex make_complex(double r, double i) {
    Complex res; res.r = r; res.i = i; return res;
}

double my_fmod(double x, double y) { 
    return x - (int)(x/y) * y; 
}

int is_operator(char c) { return strchr("+-*/^", c) != NULL; }

int validate_expression(const char *expr) {
    int len = strlen(expr);
    if (len == 0) return 1; 

    int paren_balance = 0;
    int last_was_op = 0;
    int last_was_dot = 0;
    int digit_group = 0;

    if (strchr("*/^)", expr[0])) { set_error("Error: Start with Op"); return 0; }

    for (int i = 0; i < len; i++) {
        char c = expr[i];
        if (c == '(') { paren_balance++; last_was_op=0; last_was_dot=0; digit_group=0;}
        else if (c == ')') { 
            paren_balance--; 
            if (paren_balance < 0) { set_error("Error: Extra ')'"); return 0; }
            last_was_op=0; last_was_dot=0; digit_group=0;
        }
        else if (c == '.') {
            if (last_was_dot || digit_group) { set_error("Error: Double '.'"); return 0; }
            last_was_dot = 1; digit_group = 1; 
        }
        else if (is_operator(c)) {
            if (last_was_op && c != '-') { set_error("Error: Double Op"); return 0; }
            last_was_op = 1; last_was_dot = 0; digit_group = 0; 
        }
        else {
            last_was_op = 0;
        }
    }

    if (is_operator(expr[len-1])) { set_error("Error: End with Op"); return 0; }
    if (paren_balance != 0) { set_error("Error: Unbalanced ()"); return 0; }
    return 1;
}

// raw math
double raw_sqrt(double x) {
    if (x < 0) { return 0; } 
    if (x == 0) return 0;
    double g = x / 2.0;
    for(int i=0; i<20; i++) g = (g + x/g)/2.0;
    return g;
}

double raw_exp(double x) {
    if (x > 700) { set_error("Error: Overflow"); return 0; } 
    double s=1, t=1;
    for(int i=1; i<25; i++) { t *= x/i; s += t; }
    return s;
}

double raw_ln(double x) {
    if (x <= 0) return -100000; 
    double res=0;
    while(x>2.0){ x/=MY_E; res++; }
    while(x<0.5){ x*=MY_E; res--; }
    double p=x-1, t=p, s=0;
    for(int i=1;i<20;i++){ if(i%2==1)s+=t/i; else s-=t/i; t*=p; }
    return res+s;
}

double my_pow(double base, double exp) {
    if (base == 0) return 0;
    if (exp == 0) return 1;
    if (base < 0 && exp != (int)exp) return 0; 
    return raw_exp(exp * raw_ln(my_fabs(base)));
}

double raw_sin(double x) {
    while(x > MY_PI) x -= 2*MY_PI;
    while(x < -MY_PI) x += 2*MY_PI;
    double t=x, s=x;
    for(int i=1; i<15; i++) { t = -t*x*x/((2*i)*(2*i+1)); s += t; }
    return s;
}
double raw_cos(double x) {
    while(x > MY_PI) x -= 2*MY_PI;
    while(x < -MY_PI) x += 2*MY_PI;
    double t=1, s=1;
    for(int i=1; i<15; i++) { t = -t*x*x/((2*i-1)*(2*i)); s += t; }
    return s;
}
double raw_atan(double x) { return x / (1.0 + 0.28*x*x); }
double raw_atan2(double y, double x) {
    if (x>0) return raw_atan(y/x);
    if (x<0 && y>=0) return raw_atan(y/x) + MY_PI;
    if (x<0 && y<0) return raw_atan(y/x) - MY_PI;
    if (x==0 && y>0) return MY_PI/2;
    if (x==0 && y<0) return -MY_PI/2;
    return 0;
}

// new raw hyperbolics logic
// sinh(x) = (e^x - e^-x)/2
double raw_sinh(double x) { return (raw_exp(x) - raw_exp(-x)) / 2.0; }
double raw_cosh(double x) { return (raw_exp(x) + raw_exp(-x)) / 2.0; }
double raw_tanh(double x) { return raw_sinh(x) / raw_cosh(x); }

// complex math
Complex c_add(Complex a, Complex b) { return make_complex(a.r+b.r, a.i+b.i); }
Complex c_sub(Complex a, Complex b) { return make_complex(a.r-b.r, a.i-b.i); }
Complex c_mult(Complex a, Complex b) { 
    return make_complex(a.r*b.r - a.i*b.i, a.r*b.i + a.i*b.r); 
}
Complex c_div(Complex a, Complex b) {
    double d = b.r*b.r + b.i*b.i;
    if (d == 0) { set_error("Error: Div by 0"); return make_complex(0,0); }
    return make_complex((a.r*b.r + a.i*b.i)/d, (a.i*b.r - a.r*b.i)/d);
}

// complex wrappers
Complex c_exp(Complex z) {
    double ea = raw_exp(z.r);
    return make_complex(ea * raw_cos(z.i), ea * raw_sin(z.i));
}
Complex c_ln(Complex z) {
    if (z.r==0 && z.i==0) { set_error("Error: ln(0)"); return make_complex(0,0); }
    double mag = raw_sqrt(z.r*z.r + z.i*z.i);
    double arg = raw_atan2(z.i, z.r);
    return make_complex(raw_ln(mag), arg);
}
Complex c_pow(Complex base, Complex exp) {
    if (base.r==0 && base.i==0 && exp.r <= 0) { set_error("Error: 0^0 or 0^-n"); return make_complex(0,0); }
    if (base.r==0 && base.i==0) return make_complex(0,0);
    return c_exp(c_mult(exp, c_ln(base)));
}
Complex c_sqrt(Complex z) { return c_pow(z, make_complex(0.5, 0)); }

Complex c_sin(Complex z) {
    if (angle_mode == 1 && my_fabs(z.i) < 1e-9) z.r = to_rad(z.r);
    Complex iz = make_complex(-z.i, z.r);
    Complex neg_iz = make_complex(z.i, -z.r);
    Complex num = c_sub(c_exp(iz), c_exp(neg_iz));
    return c_div(num, make_complex(0, 2));
}
Complex c_cos(Complex z) {
    if (angle_mode == 1 && my_fabs(z.i) < 1e-9) z.r = to_rad(z.r);
    Complex iz = make_complex(-z.i, z.r);
    Complex neg_iz = make_complex(z.i, -z.r);
    Complex num = c_add(c_exp(iz), c_exp(neg_iz));
    return c_div(num, make_complex(2, 0));
}
Complex c_tan(Complex z) {
    if (angle_mode == 1 && my_fabs(z.i) < 1e-9) {
        double deg = my_fmod(z.r, 180.0);
        if (my_fabs(deg - 90.0) < 1e-5 || my_fabs(deg + 90.0) < 1e-5) {
            set_error("Error: tan(90)"); return make_complex(0,0);
        }
    }
    return c_div(c_sin(z), c_cos(z));
}

// new complex hyperbolics
Complex c_sinh(Complex z) {
    // sinh(z) = sinh(x)cos(y) + i cosh(x)sin(y)
    return make_complex(raw_sinh(z.r)*raw_cos(z.i), raw_cosh(z.r)*raw_sin(z.i));
}
Complex c_cosh(Complex z) {
    // cosh(z) = cosh(x)cos(y) + i sinh(x)sin(y)
    return make_complex(raw_cosh(z.r)*raw_cos(z.i), raw_sinh(z.r)*raw_sin(z.i));
}
Complex c_tanh(Complex z) {
    return c_div(c_sinh(z), c_cosh(z));
}

Complex c_asin(Complex z) {
    if (my_fabs(z.i) < 1e-9 && my_fabs(z.r) > 1.0) { set_error("Error: asin domain"); return make_complex(0,0); }
    Complex term1 = c_mult(make_complex(0,1), z);
    Complex term2 = c_sqrt(c_sub(make_complex(1,0), c_mult(z,z)));
    Complex res = c_mult(make_complex(0,-1), c_ln(c_add(term1, term2)));
    if (angle_mode == 1) res.r = to_deg(res.r);
    return res;
}
Complex c_acos(Complex z) {
    if (my_fabs(z.i) < 1e-9 && my_fabs(z.r) > 1.0) { set_error("Error: acos domain"); return make_complex(0,0); }
    Complex res = c_sub(make_complex(MY_PI/2.0, 0), c_asin(z));
    if (angle_mode == 1) res.r = to_deg(res.r);
    return res;
}
Complex c_atan(Complex z) {
    Complex iz = c_mult(make_complex(0,1), z);
    Complex num = c_sub(make_complex(1,0), iz);
    Complex den = c_add(make_complex(1,0), iz);
    Complex t = c_div(make_complex(1,0), make_complex(0,2));
    Complex res = c_mult(t, c_ln(c_div(num, den)));
    if (angle_mode == 1) res.r = to_deg(res.r);
    return res;
}

double factorial_calc(double n) {
    if (n < 0 || n != (int)n) { set_error("Error: x! (int>=0)"); return 0; }
    if (n > 100) { set_error("Error: Overflow"); return 0; }
    double res = 1;
    for (int i = 1; i <= (int)n; i++) res *= i;
    return res;
}

void solve_linear(double a, double b, char *buf) {
    if(a==0) sprintf(buf, "Error: a=0"); else sprintf(buf, "x = %.4f", -b/a);
}
void solve_quadratic(double a, double b, double c, char *buf) {
    if (a == 0) { solve_linear(b, c, buf); return; }
    double d = b*b - 4*a*c;
    if(d>=0) sprintf(buf, "%.2f, %.2f", (-b-raw_sqrt(d))/(2*a), (-b+raw_sqrt(d))/(2*a));
    else sprintf(buf, "%.2f - %.2fi, %.2f + %.2fi", -b/(2*a), raw_sqrt(-d)/(2*a), -b/(2*a), raw_sqrt(-d)/(2*a));
}
void solve_cubic(double a, double b, double c, double d, char *buf) {
    if (a==0) { solve_quadratic(b, c, d, buf); return; }
    double A = b/a, B = c/a, C = d/a;
    double p = B - (A*A)/3.0;
    double q = (2.0*A*A*A)/27.0 - (A*B)/3.0 + C;
    double disc = (q*q)/4.0 + (p*p*p)/27.0;
    
    if (disc > 0) {
        double r = raw_sqrt(disc);
        double u_in = -q/2.0 + r;
        double v_in = -q/2.0 - r;
        double u = (u_in>0) ? my_pow(u_in, 1.0/3.0) : -my_pow(-u_in, 1.0/3.0);
        double v = (v_in>0) ? my_pow(v_in, 1.0/3.0) : -my_pow(-v_in, 1.0/3.0);
        sprintf(buf, "x = %.4f", u + v - A/3.0);
    } else {
        sprintf(buf, "Complex Roots");
    }
}

Complex parse_expr(); Complex parse_term(); Complex parse_factor(); Complex parse_power(); Complex parse_val();

Complex evaluate_expression(const char *expr) {
    clear_global_error();
    if (!validate_expression(expr)) return make_complex(0,0);
    expression_ptr = expr;
    return parse_expr();
}

Complex parse_expr() {
    Complex res = parse_term();
    while (*expression_ptr == '+' || *expression_ptr == '-') {
        char op = *expression_ptr++;
        if (op == '+') res = c_add(res, parse_term());
        else res = c_sub(res, parse_term());
    }
    return res;
}
Complex parse_term() {
    Complex res = parse_factor();
    while(1) {
        while (isspace(*expression_ptr)) expression_ptr++;
        if (*expression_ptr == '*' || *expression_ptr == '/') {
            char op = *expression_ptr++;
            if (op == '*') res = c_mult(res, parse_factor());
            else res = c_div(res, parse_factor());
        } else if (*expression_ptr == '(' || isalpha(*expression_ptr) || strncmp(expression_ptr, "π", 2)==0) {
            res = c_mult(res, parse_factor()); 
        } else break;
    }
    return res;
}
Complex parse_factor() {
    Complex res = parse_power();
    while (*expression_ptr == '^') {
        expression_ptr++;
        res = c_pow(res, parse_factor());
    }
    return res;
}
Complex parse_power() {
    Complex res = parse_val();
    while (*expression_ptr == '!') {
        expression_ptr++;
        res.r = factorial_calc(res.r);
    }
    return res;
}
Complex parse_val() {
    Complex res = {0,0};
    while (isspace(*expression_ptr)) expression_ptr++;
    int sign = 1;
    if (*expression_ptr == '-') { sign = -1; expression_ptr++; }

    if (*expression_ptr == '(') {
        expression_ptr++;
        res = parse_expr();
        if (*expression_ptr == ')') expression_ptr++;
        res.r *= sign; res.i *= sign;
        return res;
    }
    if (*expression_ptr == 'i') { expression_ptr++; return make_complex(0, sign * 1.0); }
    if (strncmp(expression_ptr, "π", 2) == 0) { expression_ptr += 2; return make_complex(sign * MY_PI, 0); }

    if (isalpha(*expression_ptr)) {
        char name[10] = {0};
        int i=0;
        while(isalpha(*expression_ptr) && i<9) name[i++] = *expression_ptr++;
        if (strcmp(name, "i")==0) return make_complex(0, sign*1.0);
        if (strcmp(name, "pi")==0) return make_complex(sign*MY_PI, 0);
        if (strcmp(name, "e")==0) return make_complex(sign*MY_E, 0);
        if (strcmp(name, "ANS")==0) return make_complex(sign*global_last_answer.r, sign*global_last_answer.i);

        if (*expression_ptr == '(') {
            expression_ptr++;
            Complex v = parse_expr();
            if (strcmp(name, "nRt")==0 && *expression_ptr == ',') {
                expression_ptr++; Complex x = parse_expr();
                if (*expression_ptr == ')') expression_ptr++;
                Complex inv_n = c_div(make_complex(1,0), v);
                res = c_pow(x, inv_n);
                res.r *= sign; res.i *= sign;
                return res;
            }
            if (*expression_ptr == ')') expression_ptr++;
            
            if (strcmp(name, "sin")==0) res = c_sin(v);
            else if (strcmp(name, "cos")==0) res = c_cos(v);
            else if (strcmp(name, "tan")==0) res = c_tan(v);
            // new hyperbolic checks
            else if (strcmp(name, "sinh")==0) res = c_sinh(v);
            else if (strcmp(name, "cosh")==0) res = c_cosh(v);
            else if (strcmp(name, "tanh")==0) res = c_tanh(v);
            
            else if (strcmp(name, "sqrt")==0) res = c_sqrt(v);
            else if (strcmp(name, "ln")==0) res = c_ln(v);
            else if (strcmp(name, "exp")==0) res = c_exp(v);
            else if (strcmp(name, "asin")==0) res = c_asin(v);
            else if (strcmp(name, "acos")==0) res = c_acos(v);
            else if (strcmp(name, "atan")==0) res = c_atan(v);
            else if (strcmp(name, "abs")==0) res = make_complex(raw_sqrt(v.r*v.r+v.i*v.i), 0);
            
            res.r *= sign; res.i *= sign;
            return res;
        }
    }
    char *end;
    res.r = strtod(expression_ptr, &end);
    if (expression_ptr == end) return make_complex(0,0);
    expression_ptr = end;
    res.r *= sign;
    return res;
}

double my_sqrt(double x){return 0;} 
double my_sin(double x){return 0;} 
double my_cos(double x){return 0;} 
double my_tan(double x){return 0;} 
double my_log(double x){return 0;} 
double my_ln(double x){return 0;} 
double my_exp(double x){return 0;} 
double my_asin(double x){return 0;} 
double my_acos(double x){return 0;} 
double my_atan(double x){return 0;}
