#include <windows.h>
#include <stdio.h>
#include <math.h>
#include "mylib.h"

// ID for the Text Box
#define ID_DISPLAY 999
#define ROWS 8
#define COLS 4

// Button Layout (Same as TUI)
char *buttons[ROWS][COLS] = {
    {"C",    "(",    ")",    "DEL"},
    {"sin",  "cos",  "tan",  "^"},
    {"asin", "acos", "atan", "sqrt"},
    {"log",  "ln",   "pi",   "/"},   // Using "pi" text for Windows compatibility
    {"7",    "8",    "9",    "*"},
    {"4",    "5",    "6",    "-"},
    {"1",    "2",    "3",    "+"},
    {"0",    ".",    "ANS",  "="}
};

// Global Input Buffer
char input_buffer[1024] = {0};
HWND hDisplay; // Handle to the text box
int result_is_displayed = 0;

// Helper: Update the display box
void update_display() {
    SetWindowText(hDisplay, input_buffer);
}

// Helper: Pretty Print (Ported from TUI)
void pretty_print_to_buffer(double result) {
    if (isnan(result) || isinf(result) || fabs(result) > 1000000000000000.0) {
        sprintf(input_buffer, "Math Error");
    }
    else if (fabs(result - 3.1415926535) <= 0.00001) {
        sprintf(input_buffer, "pi"); 
    }
    else if (result == 0.0 || (fabs(result) >= 0.0001 && fabs(result) < 1000000000000.0)) {
        sprintf(input_buffer, "%.10g", result);
    }
    else {
        // Scientific notation cleanup
        char temp[64];
        sprintf(temp, "%.4e", result);
        char *e_pos = strchr(temp, 'e');
        if (e_pos) {
            *e_pos = '\0';
            int exp_val = atoi(e_pos + 1);
            sprintf(input_buffer, "%s x 10^%d", temp, exp_val);
        } else {
            strcpy(input_buffer, temp);
        }
    }
}

// Logic: Determine if we need to add a parenthesis
int is_function(const char *btn) {
    return (strcmp(btn, "sin") == 0 || strcmp(btn, "cos") == 0 || 
            strcmp(btn, "tan") == 0 || strcmp(btn, "log") == 0 || 
            strcmp(btn, "ln") == 0  || strcmp(btn, "sqrt") == 0 ||
            strcmp(btn, "acos") == 0 || strcmp(btn, "asin") == 0 ||
            strcmp(btn, "atan") == 0 || strcmp(btn, "exp") == 0);
}

// Logic: Handle Button Clicks
void handle_button_click(const char *btn_text) {
    GetWindowText(hDisplay, input_buffer, 1024);

    if (strcmp(btn_text, "=") == 0) {
        double result = evaluate_expression(input_buffer);
        set_last_answer(result);
        pretty_print_to_buffer(result);
        result_is_displayed = 1;
    }
    else if (strcmp(btn_text, "C") == 0) {
        input_buffer[0] = '\0';
        result_is_displayed = 0;
    }
    else if (strcmp(btn_text, "DEL") == 0) {
        if (result_is_displayed) {
            input_buffer[0] = '\0';
            result_is_displayed = 0;
        } else {
            int len = strlen(input_buffer);
            if (len > 0) input_buffer[len - 1] = '\0';
        }
    }
    else {
        // Smart Clear: If showing result, clear it unless operator
        if (result_is_displayed) {
            if (strchr("+-*/^", btn_text[0]) == NULL) {
                input_buffer[0] = '\0';
            }
            result_is_displayed = 0;
        }

        // Logic to clear "Math Error"
        if (strcmp(input_buffer, "Math Error") == 0) input_buffer[0] = '\0';

        strcat(input_buffer, btn_text);
        if (is_function(btn_text)) strcat(input_buffer, "(");
    }
    update_display();
}

// The Window Procedure (Handles events)
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch(msg) {
        case WM_CREATE: {
            // 1. Create Display (Edit Control)
            hDisplay = CreateWindow("EDIT", "", 
                WS_CHILD | WS_VISIBLE | WS_BORDER | ES_RIGHT | ES_READONLY,
                10, 10, 360, 40, hwnd, (HMENU)ID_DISPLAY, NULL, NULL);

            // Change font of display to make it bigger
            HFONT hFont = CreateFont(24, 0, 0, 0, FW_BOLD, 0, 0, 0, ANSI_CHARSET, 
                                     OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, 
                                     DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "Arial");
            SendMessage(hDisplay, WM_SETFONT, (WPARAM)hFont, TRUE);

            // 2. Create Buttons Grid
            int start_y = 60;
            int btn_w = 85;
            int btn_h = 40;
            int margin = 5;

            for(int i = 0; i < ROWS; i++) {
                for(int j = 0; j < COLS; j++) {
                    int x = 10 + (j * (btn_w + margin));
                    int y = start_y + (i * (btn_h + margin));
                    
                    // Button ID = 1000 + index
                    int id = 1000 + (i * COLS + j);
                    
                    CreateWindow("BUTTON", buttons[i][j], 
                        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                        x, y, btn_w, btn_h, hwnd, (HMENU)(size_t)id, NULL, NULL);
                }
            }
            break;
        }

        case WM_COMMAND: {
            // Check if a button was clicked
            int id = LOWORD(wParam);
            if (id >= 1000 && id < 1000 + (ROWS * COLS)) {
                // Determine which button
                int idx = id - 1000;
                int r = idx / COLS;
                int c = idx % COLS;
                handle_button_click(buttons[r][c]);
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

// The Main Entry Point (WinMain instead of main)
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASSEX wc;
    HWND hwnd;
    MSG Msg;

    // 1. Register Window Class
    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = 0;
    wc.lpfnWndProc   = WndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInstance;
    wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = "Scalc_Win32";
    wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

    if(!RegisterClassEx(&wc)) {
        MessageBox(NULL, "Window Registration Failed!", "Error", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    // 2. Create the Window
    // Width: 400, Height: 480 (Adjusted for 8 rows)
    hwnd = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        "Scalc_Win32",
        "Scalc - Windows Native",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT, 400, 500,
        NULL, NULL, hInstance, NULL);

    if(hwnd == NULL) {
        MessageBox(NULL, "Window Creation Failed!", "Error", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // 3. The Message Loop
    while(GetMessage(&Msg, NULL, 0, 0) > 0) {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
    return Msg.wParam;
}
