#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <vector>
LRESULT CALLBACK FrameWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

#define button_id1 1
#define button_id2 2
#define button_id3 3
#define button_id4 4
#define button_id5 5
#define button_id7 777

#define UP 0
#define LEFT 1
#define DOWN 2
#define UPRIGHT 3
#define DOWNRIGHT 4
int MoveState = UP;

void RegClass(WNDPROC, LPCTSTR);
HWND hwndMain;
HDC hdc;

HPEN pen;
HBRUSH brush;

enum stateClientDraw {
    Stop,
    LineClientDraw,
    RectangleClientDraw,
    CirclClientDraw,
    TextClientDraw,
    CrossHair,
};

HWND edit1, edit2, edit3, edit4;
HWND edit11, edit21, edit31, edit41;
HWND editType, editType1;
enum stateClientDraw ClientDrawFigures = RectangleClientDraw;
POINT cords[2];
int RED = 255, GREEN = 0, BLUE = 0, THICK = 3, TYPE = 1;
int RED_C = 255, GREEN_C = 0, BLUE_C = 0, THICK_C = 3, TYPE_C = 1;

void ClientDraw() {
    if (ClientDrawFigures == LineClientDraw) {
        MoveToEx(hdc, cords[0].x, cords[0].y, NULL);
        LineTo(hdc, cords[1].x, cords[1].y);
    }
    else if (ClientDrawFigures == RectangleClientDraw)Rectangle(hdc, cords[0].x, cords[0].y, cords[1].x, cords[1].y);
    else if (ClientDrawFigures == CrossHair)
    {
        COLORREF Color = RGB(RED, GREEN, BLUE);
        DeleteObject(pen);
        DeleteObject(brush);
        pen = CreatePen(TYPE, THICK, Color);
        brush = CreateSolidBrush(Color);
        SelectObject(hdc, pen);
        SelectObject(hdc, brush);
        SelectObject(hdc, GetStockObject(NULL_BRUSH));

        Ellipse(hdc, cords[0].x, cords[0].y, cords[1].x, cords[1].y);

        Color = RGB(RED_C, GREEN_C, BLUE_C);
        DeleteObject(pen);
        DeleteObject(brush);
        pen = CreatePen(TYPE_C, THICK_C, Color);
        brush = CreateSolidBrush(Color);
        SelectObject(hdc, pen);
        SelectObject(hdc, brush);

        MoveToEx(hdc, (cords[1].x + cords[0].x) / 2, cords[0].y - 10, NULL);
        LineTo(hdc, (cords[1].x + cords[0].x) / 2, cords[1].y + 10);
        if (cords[0].x < cords[1].x) {
            MoveToEx(hdc, cords[0].x - 10, (cords[1].y + cords[0].y) / 2, NULL);
            LineTo(hdc, cords[1].x + 10, (cords[1].y + cords[0].y) / 2);
        }
        else {
            MoveToEx(hdc, cords[0].x + 10, (cords[1].y + cords[0].y) / 2, NULL);
            LineTo(hdc, cords[1].x - 10, (cords[1].y + cords[0].y) / 2);
        }
    }
    else if (ClientDrawFigures == CirclClientDraw)Ellipse(hdc, cords[0].x, cords[0].y, cords[1].x, cords[1].y);
    else if (ClientDrawFigures == TextClientDraw)TextOut(hdc, cords[1].x, cords[1].y, "Hello World!", 9);
}

void Arrows(HWND hwnd, WPARAM wParam, LPARAM lParam) {
    switch (wParam) {
    case VK_UP:
        cords[0].y -= 1;
        cords[1].y -= 1;
        break;
    case VK_DOWN:
        cords[0].y += 1;
        cords[1].y += 1;
        break;
    case VK_LEFT:
        cords[0].x -= 1;
        cords[1].x -= 1;
        break;
    case VK_RIGHT:
        cords[0].x += 1;
        cords[1].x += 1;
        break;
    }
    InvalidateRect(hwnd, NULL, 1);
    UpdateWindow(hwnd);
    ClientDraw();
}

void Shift(HWND hwnd, WPARAM wParam, LPARAM lParam) {
    if (wParam & MK_SHIFT) {
        int temp = abs(cords[1].x - cords[0].x);
        int temp1 = abs(cords[1].y - cords[0].y);
        cords[0].x = lParam % 0x10000;
        cords[0].y = lParam / 0x10000;
        cords[1].x = cords[0].x + temp;
        cords[1].y = cords[0].y + temp1;
        InvalidateRect(hwnd, NULL, 1);
        UpdateWindow(hwnd);
        ClientDraw();
    }
}

void Trajectory(HWND hwnd, int check = 1) {
    int flag = 0;
    int c0 = cords[0].x;
    int c1 = cords[0].y;
    int temp = abs(cords[1].x - cords[0].x);
    int temp1 = abs(cords[1].y - cords[0].y);
    while (flag != 1) {
        switch (MoveState) {
        case UP:
            if (cords[0].y <= c1 - 300) MoveState = LEFT;
            else {
                cords[0].y -= 1;
                cords[1].y = cords[0].y + temp1;
            }
            break;
        case LEFT:
            if (cords[0].x <= c0 - 300) MoveState = DOWN;
            else {
                cords[0].x -= 1;
                cords[1].x = cords[0].x + temp;
            }
            break;
        case DOWN:
            if (cords[0].y == c1) MoveState = UPRIGHT;
            else {
                cords[0].y += 1;
                cords[1].y = cords[0].y + temp1;
            }
            break;
        case UPRIGHT:
            if (cords[0].y == c1 - 150) MoveState = DOWNRIGHT;
            else {
                cords[0].x += 1.5;
                cords[0].y -= 1;
                cords[1].x = cords[0].x + temp;
                cords[1].y = cords[0].y + temp1;
            }
            break;
        case DOWNRIGHT:
            if (cords[0].y == c1) MoveState = UP;
            else {
                cords[0].x += 1.5;
                cords[0].y += 1;
                cords[1].x = cords[0].x + temp;
                cords[1].y = cords[0].y + temp1;
            }
            break;
        default: MoveState = UP;
        }
        if (check) {
            InvalidateRect(hwnd, NULL, 1);
            UpdateWindow(hwnd);
        }
        ClientDraw();
        if (cords[0].x == c0 && cords[0].y == c1) flag = 1;
    }
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSA wcl;
    memset(&wcl, 0, sizeof(WNDCLASS));
    wcl.lpszClassName = "MainWin";
    wcl.lpfnWndProc = FrameWndProc;
    wcl.hbrBackground = (HBRUSH)(COLOR_WINDOW);
    wcl.hIcon = LoadIcon(NULL, IDI_ASTERISK);
    wcl.hCursor = LoadCursor(NULL, IDC_ARROW);
    RegisterClassA(&wcl);

    hwndMain = CreateWindow("MainWin", "Practice", WS_OVERLAPPEDWINDOW, 40, 40, 1100, 620, NULL, NULL, NULL, NULL);
    ShowWindow(hwndMain, SW_SHOWNORMAL);
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}

LRESULT CALLBACK FrameWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CREATE:
    {
        hdc = GetWindowDC(hwnd);
        pen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
        HBRUSH brush = CreateSolidBrush(RGB(255, 0, 0));
        SelectObject(hdc, pen);
        SelectObject(hdc, brush);
        HWND button1 = CreateWindow("button", "LINE", WS_VISIBLE | WS_CHILD | WS_BORDER | BS_DEFPUSHBUTTON, 10, 10, 100, 30, hwnd, (HMENU)button_id1, NULL, NULL);
        HWND button2 = CreateWindow("button", "RECTANGLE", WS_VISIBLE | WS_CHILD | WS_BORDER | BS_DEFPUSHBUTTON, 10, 50, 100, 30, hwnd, (HMENU)button_id2, NULL, NULL);
        HWND button3 = CreateWindow("button", "CIRCLE", WS_VISIBLE | WS_CHILD | WS_BORDER | BS_DEFPUSHBUTTON, 10, 90, 100, 30, hwnd, (HMENU)button_id3, NULL, NULL);
        HWND button4 = CreateWindow("button", "TEXT", WS_VISIBLE | WS_CHILD | WS_BORDER | BS_DEFPUSHBUTTON, 10, 130, 100, 30, hwnd, (HMENU)button_id4, NULL, NULL);
        HWND button5 = CreateWindow("button", "CrossHair", WS_VISIBLE | WS_CHILD | WS_BORDER | BS_DEFPUSHBUTTON, 10, 170, 100, 30, hwnd, (HMENU)button_id5, NULL, NULL);

        edit1 = CreateWindow("edit", NULL, WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER, 850, 38, 30, 20, hwnd, (HMENU)11, NULL, NULL);
        edit2 = CreateWindow("edit", NULL, WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER, 900, 38, 30, 20, hwnd, (HMENU)22, NULL, NULL);
        edit3 = CreateWindow("edit", NULL, WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER, 950, 38, 30, 20, hwnd, (HMENU)33, NULL, NULL);
        HWND R = CreateWindow("static", "R", WS_VISIBLE | WS_CHILD, 850, 20, 12, 15, hwnd, NULL, NULL, NULL);
        HWND B = CreateWindow("static", "G", WS_VISIBLE | WS_CHILD, 900, 20, 12, 15, hwnd, NULL, NULL, NULL);
        HWND G = CreateWindow("static", "B", WS_VISIBLE | WS_CHILD, 950, 20, 12, 15, hwnd, NULL, NULL, NULL);

        HWND thickness = CreateWindow("static", "Thick", WS_VISIBLE | WS_CHILD, 750, 20, 40, 15, hwnd, NULL, NULL, NULL);
        edit4 = CreateWindow("edit", NULL, WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER, 750, 38, 30, 20, hwnd, (HMENU)44, NULL, NULL);
        HWND TYPE = CreateWindow("static", "Type", WS_VISIBLE | WS_CHILD, 800, 20, 40, 15, hwnd, NULL, NULL, NULL);
        editType = CreateWindow("edit", NULL, WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER, 800, 38, 30, 20, hwnd, (HMENU)444, NULL, NULL);

        edit11 = CreateWindow("edit", NULL, WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER, 850, 98, 30, 20, hwnd, (HMENU)55, NULL, NULL);
        edit21 = CreateWindow("edit", NULL, WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER, 900, 98, 30, 20, hwnd, (HMENU)66, NULL, NULL);
        edit31 = CreateWindow("edit", NULL, WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER, 950, 98, 30, 20, hwnd, (HMENU)77, NULL, NULL);
        HWND R1 = CreateWindow("static", "R2", WS_VISIBLE | WS_CHILD, 850, 70, 20, 15, hwnd, NULL, NULL, NULL);
        HWND B1 = CreateWindow("static", "G2", WS_VISIBLE | WS_CHILD, 900, 70, 20, 15, hwnd, NULL, NULL, NULL);
        HWND G1 = CreateWindow("static", "B2", WS_VISIBLE | WS_CHILD, 950, 70, 20, 15, hwnd, NULL, NULL, NULL);

        HWND thickness1 = CreateWindow("static", "Тhick2", WS_VISIBLE | WS_CHILD, 750, 70, 45, 15, hwnd, NULL, NULL, NULL);
        edit41 = CreateWindow("edit", NULL, WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER, 750, 98, 30, 20, hwnd, (HMENU)88, NULL, NULL);
        HWND TYPE1 = CreateWindow("static", "Тype2", WS_VISIBLE | WS_CHILD, 800, 70, 40, 15, hwnd, NULL, NULL, NULL);
        editType1 = CreateWindow("edit", NULL, WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER, 800, 98, 30, 20, hwnd, (HMENU)888, NULL, NULL);

        HWND button6 = CreateWindow("button", "OK", WS_VISIBLE | WS_CHILD | WS_BORDER | BS_DEFPUSHBUTTON, 1000, 68, 50, 50, hwnd, (HMENU)button_id7, NULL, NULL);
        break;
    }
    case WM_LBUTTONDOWN:
    {
        SetFocus(hwnd);
        SelectObject(hdc, GetStockObject(NULL_BRUSH));
        InvalidateRect(hwnd, 0, TRUE);
        UpdateWindow(hwnd);
        int x = LOWORD(lParam) + 5;
        int y = HIWORD(lParam) + 30;
        cords[0].x = x;
        cords[0].y = y;
        if (ClientDrawFigures == LineClientDraw) MoveToEx(hdc, x, y, NULL);
        break;
    }
    case WM_LBUTTONUP:
    {
        int x = LOWORD(lParam) + 5;
        int y = HIWORD(lParam) + 30;
        cords[1].x = x;
        cords[1].y = y;
        if (ClientDrawFigures == LineClientDraw) LineTo(hdc, x, y);
        else ClientDraw();
        break;
    }
    case WM_COMMAND:
    {
        if (LOWORD(wParam) == button_id1)
        {
            ClientDrawFigures = LineClientDraw;
        }
        else if (LOWORD(wParam) == button_id2)
        {
            ClientDrawFigures = RectangleClientDraw;
        }
        else if (LOWORD(wParam) == button_id3)
        {
            ClientDrawFigures = CirclClientDraw;
        }
        else if (LOWORD(wParam) == button_id4)
        {
            ClientDrawFigures = TextClientDraw;
        }
        else if (LOWORD(wParam) == button_id5)
        {
            ClientDrawFigures = CrossHair;
        }
        else if (LOWORD(wParam) == button_id7)
        {
            CHAR editText1[4], editText2[4], editText3[4], editText4[4], editTextType[4];
            CHAR editText5[4], editText6[4], editText7[4], editText8[4], editTextType1[4];
            GetWindowText(edit1, editText1, 4);
            RED = atoi(editText1) % 256;
            GetWindowText(edit2, editText2, 4);
            GREEN = atoi(editText2) % 256;
            GetWindowText(edit3, editText3, 4);
            BLUE = atoi(editText3) % 256;
            GetWindowText(edit4, editText4, 4);
            THICK = atoi(editText4) % 256;

            GetWindowText(editType, editTextType, 4);
            TYPE = atoi(editTextType) % 256;

            GetWindowText(edit11, editText5, 4);
            RED_C = atoi(editText5) % 256;
            GetWindowText(edit21, editText6, 4);
            GREEN_C = atoi(editText6) % 256;
            GetWindowText(edit31, editText7, 4);
            BLUE_C = atoi(editText7) % 256;
            GetWindowText(edit41, editText8, 4);
            THICK_C = atoi(editText8) % 256;

            GetWindowText(editType1, editTextType, 4);
            TYPE_C = atoi(editTextType) % 256;

            COLORREF Color = RGB(RED, GREEN, BLUE);
            DeleteObject(pen);
            DeleteObject(brush);
            pen = CreatePen(TYPE, THICK, Color);
            brush = CreateSolidBrush(Color);
            SelectObject(hdc, pen);
            SelectObject(hdc, brush);
        }
        break;
    }
    case WM_KEYDOWN:
    {
        Arrows(hwnd, wParam, lParam);
        break;
    }
    case WM_MOUSEMOVE:
        Shift(hwnd, wParam, lParam);
        break;
    case WM_RBUTTONDOWN:
        Trajectory(hwnd);
        break;
    case WM_MBUTTONDOWN:
        Trajectory(hwnd, 0);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProcA(hwnd, msg, wParam, lParam);
    }
    return DefWindowProcA(hwnd, msg, wParam, lParam);
}
