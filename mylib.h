#ifndef mylib_h
#define mylib_h

// constants
#define MY_PI 3.14159265358979323846
#define MY_E  2.71828182845904523536

typedef struct {
    double r; 
    double i; 
} Complex;

Complex evaluate_expression(const char *expression);
void set_last_answer(Complex val);

const char* get_global_error();
void clear_global_error();

void toggle_angle_mode();
int get_angle_mode(); 

void toggle_fraction_mode();
int get_fraction_mode();
int to_fraction(double val, long *num, long *den);

double my_fabs(double x);
double my_fmod(double x, double y);
Complex make_complex(double r, double i);

// raw math
double raw_sqrt(double x);
double raw_sin(double x);
double raw_cos(double x);
double raw_tan(double x);
double raw_ln(double x);
double raw_exp(double x);
double raw_atan(double x);
double raw_atan2(double y, double x);
// new raw hyperbolics
double raw_sinh(double x);
double raw_cosh(double x);
double raw_tanh(double x);

void solve_linear(double a, double b, char *buffer);
void solve_quadratic(double a, double b, double c, char *buffer);
void solve_cubic(double a, double b, double c, double d, char *buffer);

Complex c_add(Complex a, Complex b);
Complex c_sub(Complex a, Complex b);
Complex c_mult(Complex a, Complex b);
Complex c_div(Complex a, Complex b);
Complex c_pow(Complex base, Complex exp);
Complex c_sqrt(Complex z);
Complex c_ln(Complex z);
Complex c_exp(Complex z);
Complex c_sin(Complex z);
Complex c_cos(Complex z);
Complex c_tan(Complex z);
// new complex hyperbolics
Complex c_sinh(Complex z);
Complex c_cosh(Complex z);
Complex c_tanh(Complex z);

// prototypes
double my_pow(double b, double e);
double my_sqrt(double x);
double my_sin(double x);
double my_cos(double x);
double my_tan(double x);
double my_log(double x);
double my_ln(double x);
double my_exp(double x);
double my_asin(double x);
double my_acos(double x);
double my_atan(double x);

#endif
