#include <ncurses.h>
#include <string.h>
#include <math.h> 
#include <locale.h> 
#include <ctype.h>
#include <stdlib.h> 
#include "mylib.h"

#define ROWS 8
#define COLS 4
#define MAX_VAL 1000000000000000.0 

// Updated Button Layout: "DEL" -> "!"
char *buttons[ROWS][COLS] = {
    {"C",    "(",    ")",    "!"},   // Added Factorial here
    {"sin",  "cos",  "tan",  "^"},
    {"asin", "acos", "atan", "sqrt"},
    {"log",  "ln",   "π",    "/"},
    {"7",    "8",    "9",    "*"},
    {"4",    "5",    "6",    "-"},
    {"1",    "2",    "3",    "+"},
    {"0",    ".",    "ANS",  "="} 
};

char input_buffer[256] = {0};
int result_is_displayed = 0; 

void init_colors() {
    start_color();
    // Pair 1: Text(Cyan) on Background(Black)
    init_pair(1, COLOR_CYAN, COLOR_BLACK);
    // Pair 2: Selected Item (Black Text on Cyan Background)
    init_pair(2, COLOR_BLACK, COLOR_CYAN);
    // Pair 3: Error Red
    init_pair(3, COLOR_RED, COLOR_BLACK);
}

void draw_calculator(int selected_row, int selected_col) {
    clear();
    int height, width;
    getmaxyx(stdscr, height, width);

    int start_y = (height - 24) / 2;
    int start_x = (width - 50) / 2;
    
    // Draw Title with Color
    attron(COLOR_PAIR(1) | A_BOLD);
    mvprintw(start_y - 2, start_x, "SCALC SCIENTIFIC GUI");
    attroff(COLOR_PAIR(1) | A_BOLD);

    // Input Window Box
    attron(COLOR_PAIR(1));
    WINDOW *win_input = subwin(stdscr, 3, 50, start_y, start_x);
    box(win_input, 0, 0);
    
    // Check for Error text to color it red
    if (strstr(input_buffer, "Error") != NULL) wattron(win_input, COLOR_PAIR(3));
    else wattron(win_input, COLOR_PAIR(1) | A_BOLD);
    
    mvwprintw(win_input, 1, 2, "%s", input_buffer);
    wrefresh(win_input);
    attroff(COLOR_PAIR(1));

    int btn_height = 3;
    int btn_width = 12; 
    int inner_width = 10;
    int grid_start_y = start_y + 4;

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            int y = grid_start_y + (i * btn_height);
            int x = start_x + (j * btn_width);

            // Determine Color Pair
            int pair = 1;
            if (i == selected_row && j == selected_col) pair = 2;

            attron(COLOR_PAIR(pair));
            if (i == selected_row && j == selected_col) attron(A_BOLD);

            mvprintw(y, x, "+----------+");
            mvprintw(y+1, x, "|          |"); 

            char *label = buttons[i][j];
            int len = strlen(label);
            int visual_len = len;
            if (strcmp(label, "π") == 0) visual_len = 1;

            int padding = (inner_width - visual_len) / 2;
            mvprintw(y+1, x + 1 + padding, "%s", label);
            mvprintw(y+2, x, "+----------+");

            attroff(COLOR_PAIR(pair));
            attroff(A_BOLD);
        }
    }
    
    attron(COLOR_PAIR(1));
    mvprintw(height - 2, 2, "ARROWS: Navigate | ENTER: Select | KEYBOARD: Type");
    attroff(COLOR_PAIR(1));
    refresh();
}

void pretty_print_result(double val, char *buffer) {
    if (fabs(val - M_PI) <= 0.00001) { strcpy(buffer, "π"); return; }
    if (fabs(val - (-M_PI)) <= 0.00001) { strcpy(buffer, "-π"); return; }

    if (val == 0.0 || (fabs(val) >= 0.0001 && fabs(val) < 1000000000000.0)) {
        sprintf(buffer, "%.10g", val); 
        return;
    }

    char temp[64];
    sprintf(temp, "%.4e", val); 
    
    char *e_pos = strchr(temp, 'e');
    if (e_pos) {
        *e_pos = '\0'; 
        char *ptr = e_pos - 1;
        while (*ptr == '0') { *ptr = '\0'; ptr--; }
        if (*ptr == '.') *ptr = '\0';
        int exp_val = atoi(e_pos + 1);
        sprintf(buffer, "%s x 10^%d", temp, exp_val);
    } else {
        strcpy(buffer, temp);
    }
}

void perform_calculation() {
    double result = evaluate_expression(input_buffer);
    set_last_answer(result); 

    if (isnan(result) || isinf(result) || fabs(result) > MAX_VAL) {
        sprintf(input_buffer, "Math Error");
    }
    else {
        pretty_print_result(result, input_buffer);
    }
    result_is_displayed = 1;
}

int is_function(const char *btn) {
    return (strcmp(btn, "sin") == 0 || strcmp(btn, "cos") == 0 || 
            strcmp(btn, "tan") == 0 || strcmp(btn, "log") == 0 || 
            strcmp(btn, "ln") == 0  || strcmp(btn, "sqrt") == 0 ||
            strcmp(btn, "acos") == 0 || strcmp(btn, "asin") == 0 ||
            strcmp(btn, "atan") == 0 || strcmp(btn, "exp") == 0);
}

void handle_input(const char *input_str) {
    if (result_is_displayed) {
        if (strcmp(input_str, "+") == 0 || strcmp(input_str, "-") == 0 ||
            strcmp(input_str, "*") == 0 || strcmp(input_str, "/") == 0 ||
            strcmp(input_str, "^") == 0 || strcmp(input_str, "!") == 0) {
            
            if (strcmp(input_buffer, "Math Error") == 0) {
                input_buffer[0] = '\0';
                strcat(input_buffer, input_str);
            } else {
                strcat(input_buffer, input_str);
            }
        } 
        else {
            input_buffer[0] = '\0';
            strcat(input_buffer, input_str);
        }
        result_is_displayed = 0; 
    } else {
        if (strcmp(input_buffer, "Math Error") == 0) {
            input_buffer[0] = '\0';
        }
        strcat(input_buffer, input_str);
    }
}

int main() {
    setlocale(LC_ALL, ""); 
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    
    // INIT COLORS
    if (has_colors()) {
        init_colors();
    }

    int row = 0;
    int col = 0;
    int ch;

    while (1) {
        draw_calculator(row, col);
        ch = getch();

        if (ch == KEY_UP && row > 0) row--;
        else if (ch == KEY_DOWN && row < ROWS - 1) row++;
        else if (ch == KEY_LEFT && col > 0) col--;
        else if (ch == KEY_RIGHT && col < COLS - 1) col++;
        else if (ch == 'q' || ch == 27) break;

        else if (ch == 10) { 
            char *btn = buttons[row][col];
            
            if (strcmp(btn, "=") == 0) perform_calculation();
            else if (strcmp(btn, "C") == 0) {
                input_buffer[0] = '\0';
                result_is_displayed = 0;
            }
            else {
                handle_input(btn);
                if (is_function(btn)) strcat(input_buffer, "(");
            }
        }
        else if (ch == '=') perform_calculation();
        else if (ch == KEY_BACKSPACE || ch == 127) {
            if (result_is_displayed) {
                input_buffer[0] = '\0';
                result_is_displayed = 0;
            } else {
                int len = strlen(input_buffer);
                if (len > 0) input_buffer[len - 1] = '\0';
            }
        }
        else if (ch >= 32 && ch <= 126) {
            char temp[2] = {ch, '\0'};
            handle_input(temp);
        }
    }

    endwin();
    return 0;
}