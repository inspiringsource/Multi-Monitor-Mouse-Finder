#include <windows.h>
#include <shellapi.h> // For tray icon

#define ARROW_HEIGHT 40
#define ARROW_WIDTH  20
#define OVERLAY_WIDTH  30
#define OVERLAY_HEIGHT 30

const char *g_leftClass = "TrackNfindOverlayLeft";
const char *g_topClass  = "TrackNfindOverlayTop";
const char *g_trayClass = "TrackNfindTrayClass";

#define WM_TRAYICON (WM_USER + 1)
#define TRAY_ICON_ID 1

// --- Tray Icon Globals ---
NOTIFYICONDATA nid = {0};
HWND g_hwndTray = NULL;

// --- Left (Vertical) Overlay ---
int g_mouseY = 0;

void DrawLeftArrow(HDC hdc, int arrowY)
{
    POINT pts[3];
    pts[0].x = 5;                  pts[0].y = arrowY;
    pts[1].x = 5;                  pts[1].y = arrowY + ARROW_HEIGHT;
    pts[2].x = 5 + ARROW_WIDTH;    pts[2].y = arrowY + ARROW_HEIGHT / 2;

    HBRUSH hBrush = CreateSolidBrush(RGB(255, 0, 0));
    HGDIOBJ oldBrush = SelectObject(hdc, hBrush);
    Polygon(hdc, pts, 3);
    SelectObject(hdc, oldBrush);
    DeleteObject(hBrush);
}

LRESULT CALLBACK LeftWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static int scrH;
    switch(msg)
    {
        case WM_CREATE:
            scrH = GetSystemMetrics(SM_CYSCREEN); // primary monitor height
            SetTimer(hwnd, 1, 33, NULL); // approx 30 FPS
            break;
        case WM_TIMER:
        {
            POINT p;
            if (GetCursorPos(&p)) {
                RECT rc;
                GetWindowRect(hwnd, &rc);
                int arrowHeight = ARROW_HEIGHT;
                int new_mouseY = p.y - rc.top - arrowHeight / 2;
                if (new_mouseY < 0) new_mouseY = 0;
                if (new_mouseY > scrH - arrowHeight) new_mouseY = scrH - arrowHeight;
                if (new_mouseY != g_mouseY) {
                    g_mouseY = new_mouseY;
                    InvalidateRect(hwnd, NULL, FALSE);
                }
            }
        }
        break;
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            RECT rc;
            GetClientRect(hwnd, &rc);
            HBRUSH clearBrush = CreateSolidBrush(RGB(0,0,0));
            FillRect(hdc, &rc, clearBrush);
            DeleteObject(clearBrush);

            DrawLeftArrow(hdc, g_mouseY);

            EndPaint(hwnd, &ps);
        }
        break;
        case WM_DESTROY:
            KillTimer(hwnd, 1);
            break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

// --- Top (Horizontal) Overlay ---
int g_mouseX = 0;
int g_virtualX = 0; // Virtual desktop left edge

void DrawTopArrow(HDC hdc, int arrowX)
{
    POINT pts[3];
    pts[0].x = arrowX;                 pts[0].y = 5;
    pts[1].x = arrowX + ARROW_HEIGHT;  pts[1].y = 5;
    pts[2].x = arrowX + ARROW_HEIGHT/2; pts[2].y = 5 + ARROW_WIDTH;

    HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 255));
    HGDIOBJ oldBrush = SelectObject(hdc, hBrush);
    Polygon(hdc, pts, 3);
    SelectObject(hdc, oldBrush);
    DeleteObject(hBrush);
}

LRESULT CALLBACK TopWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static int virtW;
    static int virtX;
    switch(msg)
    {
        case WM_CREATE:
        {
            virtX = GetSystemMetrics(SM_XVIRTUALSCREEN);
            virtW = GetSystemMetrics(SM_CXVIRTUALSCREEN);
            g_virtualX = virtX; // For global use
             SetTimer(hwnd, 1, 33, NULL);
        }
        break;
        case WM_TIMER:
        {
            POINT p;
            if (GetCursorPos(&p)) {
                int arrowX = p.x - virtX - ARROW_HEIGHT / 2;
                if (arrowX < 0) arrowX = 0;
                if (arrowX > virtW - ARROW_HEIGHT) arrowX = virtW - ARROW_HEIGHT;
                if (arrowX != g_mouseX) {
                    g_mouseX = arrowX;
                    InvalidateRect(hwnd, NULL, FALSE);
                }
            }
        }
        break;
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            RECT rc;
            GetClientRect(hwnd, &rc);
            HBRUSH clearBrush = CreateSolidBrush(RGB(0,0,0));
            FillRect(hdc, &rc, clearBrush);
            DeleteObject(clearBrush);

            DrawTopArrow(hdc, g_mouseX);

            EndPaint(hwnd, &ps);
        }
        break;
        case WM_DESTROY:
            KillTimer(hwnd, 1);
            break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

// --- System Tray Helper Functions ---
void AddTrayIcon(HINSTANCE hInstance, HWND hwnd) {
    nid.cbSize = sizeof(NOTIFYICONDATA);
    nid.hWnd = hwnd;
    nid.uID = TRAY_ICON_ID;
    nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    nid.uCallbackMessage = WM_TRAYICON;
    nid.hIcon = LoadIcon(NULL, IDI_APPLICATION); // Default Windows app icon (change if desired)
    lstrcpy(nid.szTip, "TrackNfind (Right-click to exit)");
    Shell_NotifyIcon(NIM_ADD, &nid);
}

void RemoveTrayIcon() {
    Shell_NotifyIcon(NIM_DELETE, &nid);
}

void ShowTrayMenu(HWND hwnd, POINT pt) {
    HMENU hMenu = CreatePopupMenu();
    AppendMenu(hMenu, MF_STRING, 1, "Exit");
    SetForegroundWindow(hwnd);
    TrackPopupMenu(hMenu, TPM_BOTTOMALIGN | TPM_LEFTALIGN, pt.x, pt.y, 0, hwnd, NULL);
    DestroyMenu(hMenu);
}

// --- Tray Message Handler ---
LRESULT CALLBACK TrayWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
        case WM_TRAYICON:
            if (lParam == WM_RBUTTONUP)
            {
                POINT pt;
                GetCursorPos(&pt);
                ShowTrayMenu(hwnd, pt);
            }
            break;
        case WM_COMMAND:
            if (LOWORD(wParam) == 1) { // Exit menu
                RemoveTrayIcon();
                PostQuitMessage(0);
            }
            break;
        case WM_DESTROY:
            RemoveTrayIcon();
            break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

// --- Overlay Registration ---
void RegisterAndCreateOverlay(HINSTANCE hInstance, const char *className, WNDPROC wndProc, int x, int y, int w, int h)
{
    WNDCLASSEX wc = {0};
    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc   = wndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInstance;
    wc.hIcon         = NULL;
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(HOLLOW_BRUSH);
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = className;
    wc.hIconSm       = NULL;

    RegisterClassEx(&wc);

    HWND hwnd = CreateWindowEx(
        WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW,
        className,
        NULL,
        WS_POPUP,
        x, y, w, h,
        NULL, NULL, hInstance, NULL);

    SetLayeredWindowAttributes(hwnd, RGB(0,0,0), 0, LWA_COLORKEY);
    ShowWindow(hwnd, SW_SHOWNOACTIVATE);
    UpdateWindow(hwnd);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // Lower process priority for minimal system impact
    SetPriorityClass(GetCurrentProcess(), IDLE_PRIORITY_CLASS);

    // 1. Create hidden tray window for tray icon/messages
    WNDCLASSEX wcTray = {0};
    wcTray.cbSize = sizeof(WNDCLASSEX);
    wcTray.lpfnWndProc = TrayWndProc;
    wcTray.hInstance = hInstance;
    wcTray.lpszClassName = g_trayClass;
    RegisterClassEx(&wcTray);

    g_hwndTray = CreateWindowEx(
        0, g_trayClass, NULL, 0,
        0, 0, 0, 0, NULL, NULL, hInstance, NULL);

    AddTrayIcon(hInstance, g_hwndTray);

    // 2. Register overlays (vertical and horizontal)
    int left_x = GetSystemMetrics(SM_XVIRTUALSCREEN);
    int left_y = GetSystemMetrics(SM_YVIRTUALSCREEN);
    int left_h = GetSystemMetrics(SM_CYSCREEN); // Only primary monitor height

    RegisterAndCreateOverlay(hInstance, g_leftClass, LeftWndProc, left_x, left_y, OVERLAY_WIDTH, left_h);

    int virt_x = GetSystemMetrics(SM_XVIRTUALSCREEN);
    int virt_y = GetSystemMetrics(SM_YVIRTUALSCREEN);
    int virt_w = GetSystemMetrics(SM_CXVIRTUALSCREEN);
    RegisterAndCreateOverlay(hInstance, g_topClass, TopWndProc, virt_x, virt_y, virt_w, OVERLAY_HEIGHT);

    // 3. Standard message loop
    MSG Msg;
    while(GetMessage(&Msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
    return 0;
}
