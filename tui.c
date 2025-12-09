#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "mylib.h"
#include <locale.h> 

#define ROWS 8
#define COLS 6

// updated grid layout
char *calc_buttons[ROWS][COLS] = {
    {"SOLVE", "EXIT",  "C",    "DEL",  "(",    ")"},
    {"sin",   "asin",  "sinh", "cosh", "^",    "!"},
    {"cos",   "acos",  "tanh", "abs",  "nRt",  "/"},
    {"tan",   "atan",  "log",  "ln",   "e",    "*"},
    {"7",     "8",     "9",    "mod",  "π",    "-"},
    {"4",     "5",     "6",    "sqr",  "10^x", "+"},
    {"1",     "2",     "3",    "inv",  "ANS",  "="},
    {"0",     ".",     "i",    "F<>D", "DRG",  ""} 
};

char input_buffer[256] = {0};
int result_is_displayed = 0; 
int current_mode = 0; 
int menu_sel = 0; 

char *menu_options[] = {
    "1. Linear Equation (ax + b = 0)",
    "2. Quadratic Equation (ax^2 + bx + c = 0)",
    "3. Cubic Equation (ax^3 + ... + d = 0)",
    "4. Convert Degrees -> Radians (as factor of π)",
    "5. Convert Radians -> Degrees",
    "6. Back to Calculator"
};

void init_theme() {
    start_color();
    // simple theme black and white
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    // inverted for selection
    init_pair(2, COLOR_BLACK, COLOR_WHITE);
}

void draw_button_box(int y, int x, int h, int w, char *label, int is_sel) {
    mvhline(y, x, ACS_HLINE, w);       
    mvhline(y + h - 1, x, ACS_HLINE, w); 
    mvvline(y, x, ACS_VLINE, h);       
    mvvline(y, x + w - 1, ACS_VLINE, h); 
    mvaddch(y, x, ACS_ULCORNER);
    mvaddch(y, x + w - 1, ACS_URCORNER);
    mvaddch(y + h - 1, x, ACS_LLCORNER);
    mvaddch(y + h - 1, x + w - 1, ACS_LRCORNER);

    if (is_sel) attron(A_REVERSE);

    for (int i = 1; i < h - 1; i++) {
        mvhline(y + i, x + 1, ' ', w - 2);
    }

    int v_len = strlen(label);
    if (strcmp(label, "π") == 0) v_len = 1;
    if (strchr(label, -61)) v_len--; 
    
    int pad = (w - 2 - v_len) / 2;
    mvprintw(y + (h/2), x + 1 + pad, "%s", label);

    if (is_sel) attroff(A_REVERSE);
}

void pretty_print(Complex val, char *buf) {
    if (val.r != val.r) { sprintf(buf, "Math Error"); return; } 

    long n, d;
    int is_frac = 0;
    
    if (get_fraction_mode() && my_fabs(val.i) < 1e-9) {
        if (to_fraction(val.r, &n, &d)) {
            sprintf(buf, "%ld/%ld", n, d);
            is_frac = 1;
        }
    }

    if (!is_frac) {
        if (my_fabs(val.i) < 1e-9) {
            if (my_fabs(val.r - MY_PI) <= 1e-5) sprintf(buf, "π");
            else if (my_fabs(val.r) < 1e-9) sprintf(buf, "0");
            else sprintf(buf, "%.6g", val.r);
        } 
        else if (my_fabs(val.r) < 1e-9) {
            if (my_fabs(val.i - 1.0) < 1e-9) sprintf(buf, "i");
            else if (my_fabs(val.i + 1.0) < 1e-9) sprintf(buf, "-i");
            else sprintf(buf, "%.6gi", val.i);
        }
        else {
            sprintf(buf, "%.4g %c %.4gi", val.r, (val.i >= 0 ? '+' : '-'), my_fabs(val.i));
        }
    }
}

void draw_calc_mode(int sel_r, int sel_c) {
    clear();
    int h, w;
    getmaxyx(stdscr, h, w);
    int calc_width = 74; 
    int calc_height = 29;
    int start_y = (h - calc_height) / 2;
    int start_x = (w - calc_width) / 2;

    for(int i=-1; i<calc_height+1; i++) mvhline(start_y + i, start_x - 1, ' ', calc_width + 2);
    box(stdscr, 0, 0); 
    
    attron(A_BOLD);
    mvhline(start_y - 2, start_x, ' ', calc_width);
    char title[100];
    sprintf(title, " SCALC PRO | MODE: %s | VIEW: %s ", 
            get_angle_mode() ? "DEG" : "RAD",
            get_fraction_mode() ? "FRAC" : "DEC");
    mvprintw(start_y - 2, start_x + (calc_width-strlen(title))/2, "%s", title);
    attroff(A_BOLD);

    WINDOW *win = subwin(stdscr, 3, calc_width, start_y, start_x);
    box(win, 0, 0);
    if (strstr(input_buffer, "Error")) wattron(win, A_BOLD);
    else wattron(win, A_BOLD);
    mvwprintw(win, 1, 2, "%s", input_buffer);
    wattroff(win, A_BOLD);
    wrefresh(win);

    int btn_h = 3;
    int btn_w = 12; 

    for(int i=0; i<ROWS; i++) {
        for(int j=0; j<COLS; j++) {
            char *label = calc_buttons[i][j];
            if (strlen(label) == 0 && !(i==7 && j==5)) continue; 
            if (strcmp(label, "DRG") == 0) label = get_angle_mode() ? "DEG" : "RAD";

            int y = start_y + 4 + (i * btn_h);
            int x = start_x + (j * btn_w);
            int is_sel = (i == sel_r && j == sel_c);
            
            if (i == 6 && j == 5) {
                draw_button_box(y, x, 6, btn_w, "=", is_sel);
                continue;
            }
            if (i == 7 && j == 5) continue;

            draw_button_box(y, x, btn_h, btn_w, label, is_sel);
        }
    }
    refresh();
}

void draw_solve_menu(int selected_item) {
    clear();
    attron(A_BOLD);
    mvhline(2, 5, ' ', 60);
    mvprintw(2, 25, " SOLVER & CONVERTER ");
    attroff(A_BOLD);

    for (int i=0; i<6; i++) {
        if (i == selected_item) attron(A_REVERSE);
        mvprintw(5+i, 5, "%s", menu_options[i]);
        if (i == selected_item) attroff(A_REVERSE);
    }
    
    mvprintw(14, 5, "[ Use ARROWS to select, ENTER to confirm ]");
    refresh();
}

void do_calc() {
    if (strcasecmp(input_buffer, "exit") == 0 || strcasecmp(input_buffer, "quit") == 0) {
        endwin(); exit(0);
    }
    if (strlen(input_buffer)==0 || strchr("*/^", input_buffer[0])) return;
    
    const char* err = get_global_error();
    if (strlen(err) > 0) sprintf(input_buffer, "%s", err);
    else {
        Complex res = evaluate_expression(input_buffer);
        pretty_print(res, input_buffer);
        set_last_answer(res);
    }
    result_is_displayed = 1;
}

int main() {
    setlocale(LC_ALL, ""); 
    initscr(); cbreak(); noecho(); keypad(stdscr, TRUE); curs_set(0);
    init_theme();

    int row=0, col=0, ch;
    double a,b,c_val,d; 
    char temp_str[100]; 

    while(1) {
        if (current_mode == 0) {
            draw_calc_mode(row, col);
            ch = getch();
            if (ch == KEY_UP && row > 0) row--;
            else if (ch == KEY_DOWN) { if (row < ROWS-1) { row++; if (row == 7 && col == 5) row = 6; } }
            else if (ch == KEY_LEFT && col > 0) col--;
            else if (ch == KEY_RIGHT && col < COLS-1) col++;
            else if (ch == 10) { 
                int r = (row == 7 && col == 5) ? 6 : row;
                char *btn = calc_buttons[r][col];
                if (strcmp(btn, "=")==0) do_calc();
                else if (strcmp(btn, "C")==0) { clear_global_error(); input_buffer[0]=0; result_is_displayed=0; }
                else if (strcmp(btn, "EXIT")==0) break;
                else if (strcmp(btn, "SOLVE")==0) current_mode = 1; 
                else if (strcmp(btn, "DRG")==0) toggle_angle_mode(); 
                else if (strcmp(btn, "F<>D")==0) { 
                    toggle_fraction_mode(); 
                    if (result_is_displayed) do_calc();
                }
                else if (strcmp(btn, "DEL")==0) { int l=strlen(input_buffer); if(l>0) input_buffer[l-1]=0; }
                else {
                    if (result_is_displayed && strchr("+-*/^", btn[0])==NULL) input_buffer[0]=0;
                    if (strstr(input_buffer, "Error")) input_buffer[0] = '\0';
                    result_is_displayed = 0;
                    if (strcmp(btn,"sqr")==0) strcat(input_buffer, "^2");
                    else if (strcmp(btn,"inv")==0) strcat(input_buffer, "^-1");
                    else if (strcmp(btn,"10^x")==0) strcat(input_buffer, "10^");
                    else if (strcmp(btn,"mod")==0) strcat(input_buffer, "%");
                    else if (strcmp(btn,"nRt")==0) strcat(input_buffer, "nRt(");
                    else {
                        strcat(input_buffer, btn);
                        if (isalpha(btn[0]) && strcmp(btn,"ANS")!=0 && strcmp(btn,"nRt")!=0 
                            && strcmp(btn,"i")!=0 && strcmp(btn,"e")!=0 && strcmp(btn,"mod")!=0) 
                            strcat(input_buffer, "(");
                    }
                }
            }
            else if (ch == '=') do_calc();
            else if (ch == 'm' || ch == 'M') current_mode = 1;
            else if (ch == 127 || ch == KEY_BACKSPACE) { int l=strlen(input_buffer); if(l>0) input_buffer[l-1]=0; }
            else if (ch >= 32 && ch <= 126) {
                if (result_is_displayed && strchr("+-*/^", ch)==NULL) input_buffer[0]=0;
                if (strstr(input_buffer, "Error")) input_buffer[0] = '\0';
                result_is_displayed=0;
                int l=strlen(input_buffer); input_buffer[l]=ch; input_buffer[l+1]=0;
            }
        }
        else if (current_mode == 1) { 
            draw_solve_menu(menu_sel);
            ch = getch();
            
            if (ch == KEY_UP) {
                menu_sel--;
                if (menu_sel < 0) menu_sel = 5;
            }
            else if (ch == KEY_DOWN) {
                menu_sel++;
                if (menu_sel > 5) menu_sel = 0;
            }
            else if (ch == 'm' || ch == 'M') current_mode = 0;
            else if (ch == 10) { 
                int choice = menu_sel + 1;
                echo(); curs_set(1); clear();
                if (choice==1) {
                    mvprintw(2,2,"SOLVING: ax + b = 0"); 
                    mvprintw(4,2,"Enter a: "); scanw("%lf", &a);
                    mvprintw(5,2,"Enter b: "); scanw("%lf", &b);
                    solve_linear(a, b, input_buffer);
                } else if (choice==2) {
                    mvprintw(2,2,"SOLVING: Quadratic");
                    mvprintw(4,2,"Enter a: "); scanw("%lf", &a);
                    mvprintw(5,2,"Enter b: "); scanw("%lf", &b);
                    mvprintw(6,2,"Enter c: "); scanw("%lf", &c_val);
                    solve_quadratic(a, b, c_val, input_buffer);
                } else if (choice==3) {
                    mvprintw(2,2,"SOLVING: Cubic");
                    mvprintw(4,2,"Enter a: "); scanw("%lf", &a);
                    mvprintw(5,2,"Enter b: "); scanw("%lf", &b);
                    mvprintw(6,2,"Enter c: "); scanw("%lf", &c_val);
                    mvprintw(7,2,"Enter d: "); scanw("%lf", &d);
                    solve_cubic(a, b, c_val, d, input_buffer);
                } else if (choice==4) {
                    mvprintw(2,2,"Deg -> Rad (Supports math input)");
                    mvprintw(4,2,"Enter Degrees: "); 
                    wgetnstr(stdscr, temp_str, 99);
                    Complex res = evaluate_expression(temp_str);
                    double val = res.r; 
                    double factor = val / 180.0;
                    if (factor == 1.0) sprintf(input_buffer, "π");
                    else if (factor == -1.0) sprintf(input_buffer, "-π");
                    else if (factor == 0.0) sprintf(input_buffer, "0");
                    else sprintf(input_buffer, "%.6gπ", factor);
                } else if (choice==5) {
                    mvprintw(2,2,"Rad -> Deg (Supports 'pi')");
                    mvprintw(4,2,"Enter Radians: "); 
                    wgetnstr(stdscr, temp_str, 99);
                    Complex res = evaluate_expression(temp_str);
                    double val = res.r;
                    double deg = val * (180.0/MY_PI);
                    sprintf(input_buffer, "%.6g deg", deg);
                } else if (choice==6) {
                    // back
                }
                noecho(); curs_set(0); current_mode=0; 
            }
        }
    }
    endwin();
    return 0;
}
