#include <windows.h>
#include <vector>
#include <cmath>

// Клас "Сонце"
class Sun
{
public:
    void Draw(HDC hdc, int x, int y, int radius)
    {
        // Промені сонця
        HPEN hRayPen = CreatePen(PS_SOLID, 2, RGB(255, 170, 0));
        HPEN hOldPen = (HPEN)SelectObject(hdc, hRayPen);

        for (int i = 0; i < 12; i++)
        {
            double angle = i * (3.14159265 / 6.0);
            int x1 = x + (int)((radius + 5) * cos(angle));
            int y1 = y + (int)((radius + 5) * sin(angle));
            int x2 = x + (int)((radius + 20) * cos(angle));
            int y2 = y + (int)((radius + 20) * sin(angle));

            MoveToEx(hdc, x1, y1, NULL);
            LineTo(hdc, x2, y2);
        }

        // Тіло сонця
        HPEN hSunPen = CreatePen(PS_SOLID, 2, RGB(255, 160, 0));
        HBRUSH hSunBrush = CreateSolidBrush(RGB(255, 215, 0));
        SelectObject(hdc, hSunPen);
        HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hSunBrush);

        Ellipse(hdc, x - radius, y - radius, x + radius, y + radius);

        // Відновлення та видалення ресурсів
        SelectObject(hdc, hOldPen);
        SelectObject(hdc, hOldBrush);
        DeleteObject(hRayPen);
        DeleteObject(hSunPen);
        DeleteObject(hSunBrush);
    }
};

// Клас "Хмара"
class Cloud
{
public:
    void Draw(HDC hdc, int x, int y, double scale = 1.0)
    {
        HPEN hPen = CreatePen(PS_SOLID, 1, RGB(180, 205, 235));
        HBRUSH hBrush = CreateSolidBrush(RGB(245, 250, 255));
        HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
        HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);

        // Хмара
        int r = (int)(22 * scale);
        Ellipse(hdc, x, y, x + 2 * r, y + 2 * r);
        Ellipse(hdc, x + (int)(18 * scale), y - (int)(12 * scale), x + (int)(18 * scale) + 2 * r, y - (int)(12 * scale) + 2 * r);
        Ellipse(hdc, x + (int)(38 * scale), y - (int)(10 * scale), x + (int)(38 * scale) + 2 * r, y - (int)(10 * scale) + 2 * r);
        Ellipse(hdc, x + (int)(55 * scale), y, x + (int)(55 * scale) + 2 * r, y + 2 * r);
        Ellipse(hdc, x + (int)(25 * scale), y + (int)(5 * scale), x + (int)(25 * scale) + 2 * r, y + (int)(5 * scale) + 2 * r);

        SelectObject(hdc, hOldPen);
        SelectObject(hdc, hOldBrush);
        DeleteObject(hPen);
        DeleteObject(hBrush);
    }
};

// Клас "Будинок"
class House
{
public:
    void Draw(HDC hdc, int x, int y, int width, int height)
    {
        // 1. Стіни будинку
        HPEN hWallPen = CreatePen(PS_SOLID, 2, RGB(110, 60, 25));
        HBRUSH hWallBrush = CreateSolidBrush(RGB(222, 184, 135));
        HPEN hOldPen = (HPEN)SelectObject(hdc, hWallPen);
        HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hWallBrush);

        Rectangle(hdc, x, y, x + width, y + height);

        // 2. Дах
        HBRUSH hRoofBrush = CreateSolidBrush(RGB(178, 34, 34));
        SelectObject(hdc, hRoofBrush);

        POINT roofPoints[3] = {
            { x - 15, y },
            { x + width / 2, y - (int)(height * 0.6) },
            { x + width + 15, y }
        };
        Polygon(hdc, roofPoints, 3);

        // 3. Вікно з рамою
        HBRUSH hWindowBrush = CreateSolidBrush(RGB(173, 216, 230));
        HPEN hWindowPen = CreatePen(PS_SOLID, 2, RGB(50, 50, 50));
        SelectObject(hdc, hWindowBrush);
        SelectObject(hdc, hWindowPen);

        int winW = width / 3;
        int winH = height / 3;
        int winX = x + width / 2 - winW / 2;
        int winY = y + height / 4;

        Rectangle(hdc, winX, winY, winX + winW, winY + winH);
        // Хрестовина вікна
        MoveToEx(hdc, winX + winW / 2, winY, NULL);
        LineTo(hdc, winX + winW / 2, winY + winH);
        MoveToEx(hdc, winX, winY + winH / 2, NULL);
        LineTo(hdc, winX + winW, winY + winH / 2);

        // 4. Двері
        HBRUSH hDoorBrush = CreateSolidBrush(RGB(139, 69, 19));
        SelectObject(hdc, hDoorBrush);
        int doorW = width / 4;
        int doorH = (int)(height * 0.45);
        Rectangle(hdc, x + 15, y + height - doorH, x + 15 + doorW, y + height);

        SelectObject(hdc, hOldPen);
        SelectObject(hdc, hOldBrush);
        DeleteObject(hWallPen);
        DeleteObject(hWallBrush);
        DeleteObject(hRoofBrush);
        DeleteObject(hWindowBrush);
        DeleteObject(hWindowPen);
        DeleteObject(hDoorBrush);
    }
};

// Клас "Яблуня" (дерево з плодами)
class AppleTree
{
public:
    void Draw(HDC hdc, int x, int y, double scale = 1.0)
    {
        // 1. Стовбур
        HPEN hTrunkPen = CreatePen(PS_SOLID, 2, RGB(80, 45, 15));
        HBRUSH hTrunkBrush = CreateSolidBrush(RGB(101, 67, 33));
        HPEN hOldPen = (HPEN)SelectObject(hdc, hTrunkPen);
        HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hTrunkBrush);

        int trunkW = (int)(16 * scale);
        int trunkH = (int)(80 * scale);
        Rectangle(hdc, x - trunkW / 2, y - (int)(45 * scale), x + trunkW / 2, y + trunkH);

        // 2. Крона (листя з 3 кіл)
        HBRUSH hFoliageBrush = CreateSolidBrush(RGB(46, 139, 87));
        HPEN hFoliagePen = CreatePen(PS_SOLID, 1, RGB(34, 100, 60));
        SelectObject(hdc, hFoliageBrush);
        SelectObject(hdc, hFoliagePen);

        int r = (int)(42 * scale);
        Ellipse(hdc, x - r - (int)(12 * scale), y - (int)(55 * scale), x + (int)(15 * scale), y + (int)(5 * scale));
        Ellipse(hdc, x - (int)(15 * scale), y - (int)(55 * scale), x + r + (int)(12 * scale), y + (int)(5 * scale));
        Ellipse(hdc, x - r, y - (int)(85 * scale), x + r, y - (int)(15 * scale));

        // 3. Яблука на дереві
        HBRUSH hAppleBrush = CreateSolidBrush(RGB(220, 20, 60));
        HPEN hApplePen = CreatePen(PS_SOLID, 1, RGB(160, 0, 0));
        SelectObject(hdc, hAppleBrush);
        SelectObject(hdc, hApplePen);

        int ar = (int)(6 * scale);
        POINT apples[] = {
            { x - (int)(20 * scale), y - (int)(35 * scale) },
            { x + (int)(18 * scale), y - (int)(30 * scale) },
            { x - (int)(5 * scale),  y - (int)(55 * scale) },
            { x + (int)(2 * scale),  y - (int)(20 * scale) },
            { x - (int)(25 * scale), y - (int)(15 * scale) },
            { x + (int)(22 * scale), y - (int)(50 * scale) }
        };

        for (const auto& pt : apples)
        {
            Ellipse(hdc, pt.x - ar, pt.y - ar, pt.x + ar, pt.y + ar);
        }

        SelectObject(hdc, hOldPen);
        SelectObject(hdc, hOldBrush);
        DeleteObject(hTrunkPen);
        DeleteObject(hTrunkBrush);
        DeleteObject(hFoliagePen);
        DeleteObject(hFoliageBrush);
        DeleteObject(hApplePen);
        DeleteObject(hAppleBrush);
    }
};

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Стан інтерактивності
static int g_sunX = 120;
static int g_sunY = 100;
static int g_cloudOffset = 0;

int WINAPI WinMain(
    _In_ HINSTANCE hInst,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpszCmdLine,
    _In_ int nCmdShow)
{
    const wchar_t CLASS_NAME[] = L"SceneClass_Var26";

    WNDCLASS wc = { 0 };
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInst;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClass(&wc))
        return 0;

    HWND hwnd = CreateWindow(
        CLASS_NAME,
        L"Лабораторна 2. Завдання 2. Варіант 26: Яблуня, будинок, сонце, хмари",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT, 950, 650,
        0, 0, hInst, NULL
    );

    if (!hwnd)
        return 0;

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static int cx = 0, cy = 0;
    static Sun sun;
    static Cloud cloud;
    static House house;
    static AppleTree tree;

    switch (msg)
    {
    case WM_SIZE:
    {
        cx = LOWORD(lParam);
        cy = HIWORD(lParam);
        InvalidateRect(hwnd, NULL, TRUE); // Перемалювати при зміні розміру
        return 0;
    }

    // Обробка кліку миші (переміщення сонця у точку кліку)
    case WM_LBUTTONDOWN:
    {
        g_sunX = LOWORD(lParam);
        g_sunY = HIWORD(lParam);
        InvalidateRect(hwnd, NULL, TRUE);
        return 0;
    }

    // Обробка клавіш (рух хмар стрілками вліво/вправо або скидання пробілом)
    case WM_KEYDOWN:
    {
        switch (wParam)
        {
        case VK_LEFT:
            g_cloudOffset -= 15;
            break;
        case VK_RIGHT:
            g_cloudOffset += 15;
            break;
        case VK_SPACE:
            g_cloudOffset = 0;
            g_sunX = 120;
            g_sunY = 100;
            break;
        }
        InvalidateRect(hwnd, NULL, TRUE);
        return 0;
    }

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        // 1. ТЛО: Небо
        HBRUSH hSkyBrush = CreateSolidBrush(RGB(185, 225, 255));
        RECT skyRect = { 0, 0, cx, (int)(cy * 0.65) };
        FillRect(hdc, &skyRect, hSkyBrush);
        DeleteObject(hSkyBrush);

        // 2. ТЛО: Трава
        HBRUSH hGroundBrush = CreateSolidBrush(RGB(110, 185, 80));
        RECT groundRect = { 0, (int)(cy * 0.65), cx, cy };
        FillRect(hdc, &groundRect, hGroundBrush);
        DeleteObject(hGroundBrush);

        // Лінія горизонту
        HPEN hHorizonPen = CreatePen(PS_SOLID, 2, RGB(80, 150, 60));
        HPEN hOldPen = (HPEN)SelectObject(hdc, hHorizonPen);
        MoveToEx(hdc, 0, (int)(cy * 0.65), NULL);
        LineTo(hdc, cx, (int)(cy * 0.65));
        SelectObject(hdc, hOldPen);
        DeleteObject(hHorizonPen);

        // 3. СОНЦЕ
        sun.Draw(hdc, g_sunX, g_sunY, 35);

        // 4. ХМАРИ
        cloud.Draw(hdc, 220 + g_cloudOffset, 60, 1.1);
        cloud.Draw(hdc, 500 + (int)(g_cloudOffset * 0.8), 85, 0.9);
        cloud.Draw(hdc, 720 + (int)(g_cloudOffset * 1.2), 50, 1.2);

        int groundY = (int)(cy * 0.65);

        // 5. БУДИНОК
        house.Draw(hdc, (int)(cx * 0.38), groundY - 140, 170, 140);

        // 6. ЯБЛУНІ
        tree.Draw(hdc, (int)(cx * 0.18), groundY - 40, 1.2);  // Велика яблуня зліва
        tree.Draw(hdc, (int)(cx * 0.72), groundY - 30, 1.1);  // Яблуня справа від будинку
        tree.Draw(hdc, (int)(cx * 0.88), groundY - 15, 0.9);  // Менша яблуня вдалині

        // 7. Підказка для керування
        SetBkMode(hdc, TRANSPARENT);
        SetTextColor(hdc, RGB(50, 50, 50));
        const wchar_t tip[] = L"Керування: ЛКМ — перемістити сонце; Стрілки Вліво/Вправо — рух хмар; Пробіл — скидання";
        TextOutW(hdc, 15, cy - 28, tip, (int)wcslen(tip));

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
