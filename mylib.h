#ifndef mylib_h
#define mylib_h

double evaluate_expression(const char *expression);

// New: Store the answer so the parser can use "ANS"
void set_last_answer(double val);

#endif