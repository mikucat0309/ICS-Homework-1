/*
 * ============================================================
 * Author: Mikucat
 * Environment: Windows10-1903
 * Toolchain: x86_64-posix-seh-rev0, Built by MinGW-W64 project
 * Version: GUI ver. 1.2
 * ============================================================
 */

#include <windows.h>
#include <ctype.h>
#include "convert.h"

#define true 1
#define false 0

static char szWindowClass[] = "DesktopApp";

static char szTitle[] = "Mikucat's Homework #1";

HINSTANCE hInst;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

LRESULT CALLBACK KeyboardCallback(int code, WPARAM wParam, LPARAM lParam);

int isInvalidBase(int base) {
    return (!base || base < 2 || base > 36);
}

char *calculate(int originBase, int targetBase, char *input) {
    int flag = false;
    char *szResult;
    ull value;

    if (isInvalidBase(originBase) || isInvalidBase(targetBase) || !input[0])
        return NULL;
    if (input[0] == '-')
        flag = true;
    value = n2dec(input + flag, originBase);
    if (value == -1)
        return NULL;
    szResult = dec2n(value, targetBase, flag);
    return szResult;
}

void GetIntFromText(HWND hWnd, int *value) {
    int len = GetWindowTextLengthA(hWnd) + 1;
    char *tmp = calloc(len, sizeof(char));
    GetWindowTextA(hWnd, tmp, len);
    sscanf(tmp, "%d", value);
}

void SetIntToText(HWND hWnd, int value) {
    char tmp[100];
    sprintf(tmp, "%d", value);
    SetWindowTextA(hWnd, tmp);
}

int CALLBACK WinMain(
        HINSTANCE hInstance,
        HINSTANCE hPrevInstance,
        LPSTR lpCmdLine,
        int nCmdShow) {
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

    if (!RegisterClassEx(&wcex)) {
        MessageBox(
                NULL,
                "Call to RegisterClassEx failed!",
                szTitle,
                0L);

        return 1;
    }

    hInst = hInstance;

    HWND hWnd = CreateWindow(
            szWindowClass,
            szTitle,
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT,
            330, 250,
            NULL,
            NULL,
            hInstance,
            NULL);

    if (!hWnd) {
        MessageBox(
                NULL,
                "Call to CreateWindow failed!",
                "Windows Desktop Guided Tour",
                0L);

        return 1;
    }
    printf("hooking...\n");
    SetWindowsHookEx(WH_KEYBOARD, KeyboardCallback, hInstance, GetCurrentProcessId());

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int) msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    PAINTSTRUCT ps;
    HDC hdc;
    static HWND hWndBase1, hWndBase2, hWndValue1, hWndValue2, hwndButton1, hwndButton2, hwndButton3;
    static TCHAR response[100] = {0};
    static int x = 10;
    static int y = 10;

    switch (message) {
        case WM_PAINT:
            hdc = BeginPaint(hWnd, &ps);
            x = 10;
            y = 10;
            y += 40;
            TextOut(hdc, x, y, response, _tcslen(response));
            y += 20;
            x += 50;
            TextOut(hdc, x, y, _T("base"), _tcslen(_T("base")));
            x += 40;
            TextOut(hdc, x, y, _T("input"), _tcslen(_T("input")));

            x = 10 + 50;
            y += 50;
            TextOut(hdc, x, y, _T("base"), _tcslen(_T("base")));
            x += 40;
            TextOut(hdc, x, y, _T("result"), _tcslen(_T("result")));

            EndPaint(hWnd, &ps);
            break;
        case WM_CREATE:
            hwndButton1 = CreateWindowEx(
                    WS_EX_CLIENTEDGE,
                    "BUTTON", "Change base",
                    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, // Styles
                    x, y, 100, 30,
                    hWnd, (HMENU) 1, (HINSTANCE) GetWindowLongPtr(hWnd, GWLP_WNDPROC), NULL);
            x += 110;
            hwndButton2 = CreateWindowEx(
                    WS_EX_CLIENTEDGE,
                    "BUTTON", "Calculate",
                    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, // Styles
                    x, y, 80, 30,
                    hWnd, (HMENU) 2, (HINSTANCE) GetWindowLongPtr(hWnd, GWLP_WNDPROC), NULL);
            x += 90;
            hwndButton3 = CreateWindowEx(
                    WS_EX_CLIENTEDGE,
                    "BUTTON", "Reset",
                    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, // Styles
                    x, y, 80, 30,
                    hWnd, (HMENU) 3, (HINSTANCE) GetWindowLongPtr(hWnd, GWLP_WNDPROC), NULL);
            x = 10;
            y += 80;

            x += 50;
            hWndBase1 = CreateWindowEx(
                    WS_EX_CLIENTEDGE,
                    "EDIT", "2",
                    WS_CHILD | WS_VISIBLE,
                    x, y, 30, 25,
                    hWnd, NULL, NULL, NULL);
            x += 40;
            hWndValue1 = CreateWindowEx(
                    WS_EX_CLIENTEDGE,
                    "EDIT", "0",
                    WS_CHILD | WS_VISIBLE,
                    x, y, 150, 25,
                    hWnd, NULL, NULL, NULL);
            x = 10;
            y += 50;

            x += 50;
            hWndBase2 = CreateWindowEx(
                    WS_EX_CLIENTEDGE,
                    "EDIT", "10",
                    WS_CHILD | WS_VISIBLE,
                    x, y, 30, 25,
                    hWnd, NULL, NULL, NULL);
            x += 40;
            hWndValue2 = CreateWindowEx(
                    WS_EX_CLIENTEDGE,
                    "EDIT", "",
                    WS_CHILD | WS_VISIBLE,
                    x, y, 150, 25,
                    hWnd, NULL, NULL, NULL);
            x = 10;
            y += 30;
            break;
        case WM_COMMAND:
            if (HIWORD(wParam) == BN_CLICKED) {
                int base1, base2;
                switch (LOWORD(wParam)) {
                    case 1:
                        GetIntFromText(hWndBase1, &base1);
                        GetIntFromText(hWndBase2, &base2);
                        SetIntToText(hWndBase1, base2);
                        SetIntToText(hWndBase2, base1);
                        break;
                    case 2: {
                        char szValue1[50];
                        int len = GetWindowTextLengthA(hWndValue1) + 1;
                        GetIntFromText(hWndBase1, &base1);
                        GetIntFromText(hWndBase2, &base2);
                        GetWindowTextA(hWndValue1, szValue1, len);

                        for (char *p = szValue1; *p; ++p)
                            *p = (char) toupper(*p);

                        // easter egg
                        if (!strcmp(szValue1, "MIKU")) {
                            SetWindowTextA(hWndValue2, "0831");
                            SetWindowTextA(hWndBase1, "39");
                            SetWindowTextA(hWndBase2, "39");
                            _tcscpy(response, "Miku My Waifu!");
                            break;
                        }
                        char *output = calculate(base1, base2, szValue1);
                        if (output != NULL) {
                            SetWindowTextA(hWndValue2, output);
                            _tcscpy(response, "Success");
                            free(output);
                        } else {
                            SetWindowTextA(hWndValue2, "Error");
                            _tcscpy(response, "Error");
                        }
                        break;
                    }
                    case 3:
                        SetWindowTextA(hWndBase1, "2");
                        SetWindowTextA(hWndBase2, "10");
                        SetWindowTextA(hWndValue1, "0");
                        SetWindowTextA(hWndValue2, "");
                        _tcscpy(response, "Reset");
                        break;
                    default:
                        break;
                }
                InvalidateRect(hWnd, NULL, TRUE);
                UpdateWindow(hWnd);
            }
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}

LRESULT CALLBACK KeyboardCallback(int code, WPARAM wParam, LPARAM lParam) {
    printf("meow\n");
    return CallNextHookEx(0, code, wParam, lParam);
}