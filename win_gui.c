#include <windows.h>
#include <stdio.h>
#include <math.h>
#include "mylib.h"

// ID definitions for ui elements
#define ID_DISPLAY 100
#define ID_GRID_START 1000

// grid dimensions
#define ROWS 8
#define COLS 6

// layout matches the tui version exactly
char *calc_buttons[ROWS][COLS] = {
    {"SOLVE", "EXIT",  "C",    "DEL",  "(",    ")"},
    {"sin",   "asin",  "floor","ceil", "^",    "!"},
    {"cos",   "acos",  "round","abs",  "nRt",  "/"},
    {"tan",   "atan",  "log",  "ln",   "e",    "*"},
    {"7",     "8",     "9",    "mod",  "π",    "-"},
    {"4",     "5",     "6",    "sqr",  "10^x", "+"},
    {"1",     "2",     "3",    "inv",  "ANS",  "="},
    {"0",     ".",     "i",    "F<>D", "DRG",  ""} 
};

// global state
HWND hDisplay;
HWND hButtons[ROWS][COLS];
char input_buffer[256] = {0};
int result_is_displayed = 0;
int solver_mode = 0; // 0=calc, 1=linear, 2=quad, 3=cubic, 4=d2r, 5=r2d

// dark mode colors
const COLORREF BG_COLOR = RGB(30, 30, 30);
const COLORREF BTN_COLOR = RGB(50, 50, 50);
const COLORREF TEXT_COLOR = RGB(255, 255, 255);
const COLORREF ACCENT_COLOR = RGB(0, 120, 215); // for = button
const COLORREF DANGER_COLOR = RGB(200, 50, 50); // for C/Exit

// helper to format the output nicely
void update_display() {
    SetWindowText(hDisplay, input_buffer);
}

// pretty printer ported from tui
void pretty_print_win(Complex val) {
    char buf[256];
    
    // fraction mode check
    if (get_fraction_mode() && my_fabs(val.i) < 1e-9) {
        long n, d;
        if (to_fraction(val.r, &n, &d)) {
            sprintf(buf, "%ld/%ld", n, d);
            strcpy(input_buffer, buf);
            return;
        }
    }

    if (val.r != val.r) strcpy(buf, "Math Error");
    else if (my_fabs(val.i) < 1e-9) {
        if (my_fabs(val.r - MY_PI) <= 1e-5) strcpy(buf, "π");
        else if (my_fabs(val.r) < 1e-9) strcpy(buf, "0");
        else sprintf(buf, "%.6g", val.r);
    }
    else if (my_fabs(val.r) < 1e-9) {
        sprintf(buf, "%.6gi", val.i);
    }
    else {
        sprintf(buf, "%.4g %c %.4gi", val.r, (val.i >= 0 ? '+' : '-'), my_fabs(val.i));
    }
    strcpy(input_buffer, buf);
}

// parsing logic for the solver inputs (e.g. "1 2 3")
void handle_solver() {
    double args[4] = {0};
    int count = 0;
    char *token = strtok(input_buffer, " ,");
    while(token != NULL && count < 4) {
        args[count++] = atof(token);
        token = strtok(NULL, " ,");
    }

    char res_buf[256];
    if (solver_mode == 1) solve_linear(args[0], args[1], res_buf);
    else if (solver_mode == 2) solve_quadratic(args[0], args[1], args[2], res_buf);
    else if (solver_mode == 3) solve_cubic(args[0], args[1], args[2], args[3], res_buf);
    else if (solver_mode == 4) sprintf(res_buf, "%.6g rad", to_rad(args[0]));
    else if (solver_mode == 5) sprintf(res_buf, "%.6g deg", to_deg(args[0]));
    
    strcpy(input_buffer, res_buf);
    solver_mode = 0; // go back to calc mode
}

// main button logic
void handle_click(char *btn) {
    // if we are in solver mode waiting for input
    if (solver_mode > 0 && strcmp(btn, "=") == 0) {
        handle_solver();
        update_display();
        result_is_displayed = 1;
        return;
    }

    if (strcmp(btn, "=") == 0) {
        if (strlen(input_buffer) == 0) return;
        Complex res = evaluate_expression(input_buffer);
        set_last_answer(res);
        pretty_print_win(res);
        result_is_displayed = 1;
    }
    else if (strcmp(btn, "C") == 0) {
        input_buffer[0] = '\0';
        solver_mode = 0;
        result_is_displayed = 0;
    }
    else if (strcmp(btn, "EXIT") == 0) {
        PostQuitMessage(0);
    }
    else if (strcmp(btn, "DRG") == 0) {
        toggle_angle_mode();
        // update button text to show current mode
        SetWindowText(hButtons[7][4], get_angle_mode() ? "DEG" : "RAD");
        return; 
    }
    else if (strcmp(btn, "F<>D") == 0) {
        toggle_fraction_mode();
        // if showing a result refresh it
        if (result_is_displayed) {
            // re-eval last answer logic is tricky here so i just ignore visual update
            // until next calc usually but user can press = again
        }
    }
    else if (strcmp(btn, "SOLVE") == 0) {
        // create a popup menu for solvers
        HMENU hMenu = CreatePopupMenu();
        AppendMenu(hMenu, MF_STRING, 1, "1. Linear (ax+b=0)");
        AppendMenu(hMenu, MF_STRING, 2, "2. Quadratic (ax^2+bx+c=0)");
        AppendMenu(hMenu, MF_STRING, 3, "3. Cubic");
        AppendMenu(hMenu, MF_STRING, 4, "4. Deg -> Rad");
        AppendMenu(hMenu, MF_STRING, 5, "5. Rad -> Deg");
        
        POINT p; GetCursorPos(&p);
        int sel = TrackPopupMenu(hMenu, TPM_RETURNCMD | TPM_NONOTIFY, p.x, p.y, 0, hDisplay, NULL);
        DestroyMenu(hMenu);
        
        if (sel > 0) {
            solver_mode = sel;
            if (sel == 1) strcpy(input_buffer, "Enter a b: ");
            if (sel == 2) strcpy(input_buffer, "Enter a b c: ");
            if (sel == 3) strcpy(input_buffer, "Enter a b c d: ");
            if (sel == 4) strcpy(input_buffer, "Enter Deg: ");
            if (sel == 5) strcpy(input_buffer, "Enter Rad: ");
            result_is_displayed = 0; // let user type
        }
    }
    else if (strcmp(btn, "DEL") == 0) {
        int len = strlen(input_buffer);
        if (len > 0) input_buffer[len-1] = '\0';
    }
    else {
        // typing logic
        if (result_is_displayed && strchr("+-*/^", btn[0]) == NULL) {
            input_buffer[0] = '\0';
        }
        
        // special text handling
        if (strstr(input_buffer, "Enter")) input_buffer[0] = '\0'; // clear prompt text
        
        if (strcmp(btn, "sqr")==0) strcat(input_buffer, "^2");
        else if (strcmp(btn, "inv")==0) strcat(input_buffer, "^-1");
        else if (strcmp(btn, "10^x")==0) strcat(input_buffer, "10^");
        else if (strcmp(btn, "mod")==0) strcat(input_buffer, "%");
        else if (strcmp(btn, "nRt")==0) strcat(input_buffer, "nRt(");
        else {
            strcat(input_buffer, btn);
            if (isalpha(btn[0]) && strcmp(btn,"ANS")!=0 && strcmp(btn,"nRt")!=0 
                && strcmp(btn,"i")!=0 && strcmp(btn,"e")!=0 && strcmp(btn,"mod")!=0) {
                strcat(input_buffer, "(");
            }
        }
        result_is_displayed = 0;
    }
    update_display();
}

// owner draw button to make them dark
void draw_item(LPDRAWITEMSTRUCT pDIS) {
    char text[32];
    GetWindowText(pDIS->hwndItem, text, 32);
    
    // choose color
    COLORREF bg = BTN_COLOR;
    if (strcmp(text, "=")==0) bg = ACCENT_COLOR;
    if (strcmp(text, "C")==0 || strcmp(text, "EXIT")==0) bg = DANGER_COLOR;
    if (pDIS->itemState & ODS_SELECTED) bg = RGB(100, 100, 100);

    HBRUSH brush = CreateSolidBrush(bg);
    FillRect(pDIS->hDC, &pDIS->rcItem, brush);
    DeleteObject(brush);
    
    SetBkMode(pDIS->hDC, TRANSPARENT);
    SetTextColor(pDIS->hDC, TEXT_COLOR);
    DrawText(pDIS->hDC, text, -1, &pDIS->rcItem, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch(msg) {
        case WM_CREATE: {
            // create display
            hDisplay = CreateWindow("EDIT", "", 
                WS_CHILD | WS_VISIBLE | WS_BORDER | ES_RIGHT | ES_READONLY,
                10, 10, 460, 40, hwnd, (HMENU)ID_DISPLAY, NULL, NULL);
            
            // set font
            HFONT hFont = CreateFont(24,0,0,0,FW_BOLD,0,0,0,0,0,0,0,0,"Consolas");
            SendMessage(hDisplay, WM_SETFONT, (WPARAM)hFont, 0);

            // create grid
            int w = 70, h = 40, gap = 5;
            for(int r=0; r<ROWS; r++) {
                for(int c=0; c<COLS; c++) {
                    if (strlen(calc_buttons[r][c]) == 0) continue;
                    
                    int x = 10 + c*(w+gap);
                    int y = 60 + r*(h+gap);
                    
                    // handle big equals
                    int bh = h;
                    if (r==6 && c==5) bh = h*2 + gap;
                    
                    hButtons[r][c] = CreateWindow("BUTTON", calc_buttons[r][c],
                        WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
                        x, y, w, bh, hwnd, (HMENU)(size_t)(ID_GRID_START + r*COLS + c), NULL, NULL);
                }
            }
            break;
        }
        case WM_DRAWITEM: // custom button colors
            draw_item((LPDRAWITEMSTRUCT)lParam);
            break;
            
        case WM_COMMAND: {
            int id = LOWORD(wParam);
            if (id >= ID_GRID_START) {
                int idx = id - ID_GRID_START;
                int r = idx / COLS;
                int c = idx % COLS;
                handle_click(calc_buttons[r][c]);
            }
            break;
        }
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR args, int nShow) {
    WNDCLASS wc = {0};
    wc.lpszClassName = "ScalcWindow";
    wc.lpfnWndProc = WndProc;
    wc.hbrBackground = CreateSolidBrush(BG_COLOR);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    RegisterClass(&wc);

    HWND hwnd = CreateWindow("ScalcWindow", "Scalc Pro (Windows)", 
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        100, 100, 500, 500, NULL, NULL, hInst, NULL); // Adjusted height for 8 rows

    ShowWindow(hwnd, nShow);
    
    MSG msg;
    while(GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}
