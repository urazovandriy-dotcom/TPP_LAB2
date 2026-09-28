#include <windows.h>
#include <cmath>
#include <cwchar> 

BOOL RegClass(WNDPROC, LPCTSTR, UINT);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

HINSTANCE hInstance;
wchar_t szClassName[] = L"GraphClass_Var26";

const int NUM_POINTS = 200;

typedef struct
{
    wchar_t name[64];
    float x[NUM_POINTS];
    float y[NUM_POINTS];
} FUNC;

int WINAPI WinMain(
    _In_ HINSTANCE hInst,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpszCmdLine,
    _In_ int nCmdShow)
{
    MSG msg;
    HWND hwnd;
    ::hInstance = hInst;

    if (!RegClass(WndProc, szClassName, COLOR_WINDOW))
        return FALSE;

    hwnd = CreateWindow(
        szClassName,
        L"Лабораторна 2. Завдання 1. Варіант 26",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT, 900, 600,
        0, 0, hInst, NULL
    );

    if (!hwnd)
        return FALSE;

    while (GetMessage(&msg, 0, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}

BOOL RegClass(WNDPROC Proc, LPCTSTR szName, UINT brBackground)
{
    WNDCLASS wc = { 0 };
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = Proc;
    wc.hInstance = hInstance;
    wc.lpszClassName = szName;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(brBackground + 1);
    return (RegisterClass(&wc) != 0);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static short cx, cy;
    static FUNC myfunc;

    switch (msg)
    {
    case WM_SIZE:
    {
        cx = LOWORD(lParam);
        cy = HIWORD(lParam);
        return 0;
    }
    case WM_CREATE:
    {
        wcscpy_s(myfunc.name, 64, L"y = sin(2x) + cos(x^2),  x in [0; 3]");

        // Заповнення масиву точок для відрізка [0; 3]
        float x_start = 0.0f;
        float x_end = 3.0f;
        float step = (x_end - x_start) / (NUM_POINTS - 1);

        for (int i = 0; i < NUM_POINTS; i++)
        {
            myfunc.x[i] = x_start + i * step;
            // Обчислення функції y = sin(2x) + cos(x^2)
            myfunc.y[i] = (float)(std::sin(2.0 * myfunc.x[i]) + std::cos(myfunc.x[i] * myfunc.x[i]));
        }
        return 0;
    }
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        // Відступи від країв вікна
        int leftMargin = 70;
        int rightMargin = 50;
        int topMargin = 70;
        int bottomMargin = 60;

        int plotWidth = cx - leftMargin - rightMargin;
        int plotHeight = cy - topMargin - bottomMargin;

        if (plotWidth <= 0 || plotHeight <= 0)
        {
            EndPaint(hwnd, &ps);
            return 0;
        }

        // Межі значень функції
        float x_min = myfunc.x[0];
        float x_max = myfunc.x[NUM_POINTS - 1];
        float y_min = myfunc.y[0];
        float y_max = myfunc.y[0];

        for (int i = 1; i < NUM_POINTS; i++)
        {
            if (myfunc.y[i] > y_max) y_max = myfunc.y[i];
            if (myfunc.y[i] < y_min) y_min = myfunc.y[i];
        }

        float y_range = (y_max - y_min);
        float y_display_max = y_max + 0.15f * y_range;
        float y_display_min = y_min - 0.15f * y_range;

        // Лямбда-функції для перетворення координат з математичних у пікселі вікна
        auto toScreenX = [&](float x) -> int {
            return leftMargin + (int)((x - x_min) / (x_max - x_min) * plotWidth);
            };
        auto toScreenY = [&](float y) -> int {
            return topMargin + (int)((y_display_max - y) / (y_display_max - y_display_min) * plotHeight);
            };

        SetBkMode(hdc, TRANSPARENT);

        // 1. Виведення назви графіка
        HFONT hFontTitle = CreateFont(20, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
            DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Segoe UI");
        HFONT hOldFont = (HFONT)SelectObject(hdc, hFontTitle);

        TextOutW(hdc, leftMargin, 15, myfunc.name, (int)wcslen(myfunc.name));

        // 2. Малювання осей координат (перо чорного кольору)
        HPEN hAxisPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
        HPEN hOldPen = (HPEN)SelectObject(hdc, hAxisPen);

        int axisY_X = toScreenX(0.0f); // Положення осі Y (x = 0)
        int axisX_Y = toScreenY(0.0f); // Положення осі X (y = 0)

        // Вісь OY
        MoveToEx(hdc, axisY_X, cy - bottomMargin + 20, NULL);
        LineTo(hdc, axisY_X, topMargin - 25);
        // Стрілка на осі OY
        LineTo(hdc, axisY_X - 5, topMargin - 15);
        MoveToEx(hdc, axisY_X, topMargin - 25, NULL);
        LineTo(hdc, axisY_X + 5, topMargin - 15);
        TextOutW(hdc, axisY_X - 25, topMargin - 30, L"y", 1);

        // Вісь OX
        MoveToEx(hdc, leftMargin - 20, axisX_Y, NULL);
        LineTo(hdc, cx - rightMargin + 25, axisX_Y);
        // Стрілка на осі OX
        LineTo(hdc, cx - rightMargin + 15, axisX_Y - 5);
        MoveToEx(hdc, cx - rightMargin + 25, axisX_Y, NULL);
        LineTo(hdc, cx - rightMargin + 15, axisX_Y + 5);
        TextOutW(hdc, cx - rightMargin + 30, axisX_Y - 8, L"x", 1);

        // 3. Сітка, поділки та підписи осей
        HPEN hGridPen = CreatePen(PS_DOT, 1, RGB(200, 200, 200));
        HFONT hFontLabels = CreateFont(15, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
            DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Segoe UI");
        SelectObject(hdc, hFontLabels);

        // Поділки по осі OX
        for (float x_val = 0.5f; x_val <= 3.01f; x_val += 0.5f)
        {
            int sx = toScreenX(x_val);

            // Пунктирна лінія сітки
            SelectObject(hdc, hGridPen);
            MoveToEx(hdc, sx, topMargin, NULL);
            LineTo(hdc, sx, cy - bottomMargin);

            // Засічка на осі OX
            SelectObject(hdc, hAxisPen);
            MoveToEx(hdc, sx, axisX_Y - 4, NULL);
            LineTo(hdc, sx, axisX_Y + 4);

            // Текстовий підпис
            wchar_t buf[16];
            swprintf_s(buf, L"%.1f", x_val);
            TextOutW(hdc, sx - 10, axisX_Y + 7, buf, (int)wcslen(buf));
        }

        // Поділки по осі OY
        for (float y_val = -2.0f; y_val <= 2.01f; y_val += 0.5f)
        {
            if (std::abs(y_val) < 0.001f)
            {
                // Підпис початку координат 0
                TextOutW(hdc, axisY_X - 16, axisX_Y + 5, L"0", 1);
                continue;
            }

            int sy = toScreenY(y_val);
            if (sy < topMargin || sy > cy - bottomMargin)
                continue;

            // Пунктирна лінія сітки
            SelectObject(hdc, hGridPen);
            MoveToEx(hdc, leftMargin, sy, NULL);
            LineTo(hdc, cx - rightMargin, sy);

            // Засічка на осі OY
            SelectObject(hdc, hAxisPen);
            MoveToEx(hdc, axisY_X - 4, sy, NULL);
            LineTo(hdc, axisY_X + 4, sy);

            // Текстовий підпис
            wchar_t buf[16];
            swprintf_s(buf, L"%.1f", y_val);
            TextOutW(hdc, axisY_X - 42, sy - 8, buf, (int)wcslen(buf));
        }

        // 4. Малювання графіка функції
        HPEN hGraphPen = CreatePen(PS_SOLID, 2, RGB(0, 102, 204));
        SelectObject(hdc, hGraphPen);

        MoveToEx(hdc, toScreenX(myfunc.x[0]), toScreenY(myfunc.y[0]), NULL);
        for (int i = 1; i < NUM_POINTS; i++)
        {
            LineTo(hdc, toScreenX(myfunc.x[i]), toScreenY(myfunc.y[i]));
        }

        // Очищення виділених ресурсів GDI
        SelectObject(hdc, hOldPen);
        SelectObject(hdc, hOldFont);

        DeleteObject(hAxisPen);
        DeleteObject(hGridPen);
        DeleteObject(hGraphPen);
        DeleteObject(hFontTitle);
        DeleteObject(hFontLabels);

        EndPaint(hwnd, &ps);
        return 0;
    }
    case WM_DESTROY:
    {
        PostQuitMessage(0);
        return 0;
    }
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}