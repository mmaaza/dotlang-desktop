// DotLangDesktop.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "DotLangDesktop.h"
#include <commctrl.h>
#include <uxtheme.h>

#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "uxtheme.lib")

#define MAX_LOADSTRING 100

// Modern color palette
#define COLOR_BG_MAIN       RGB(250, 251, 252)     // Very light blue-gray
#define COLOR_BG_PANEL      RGB(255, 255, 255)     // White
#define COLOR_BORDER        RGB(218, 225, 231)     // Light border
#define COLOR_BUTTON_BG     RGB(36, 41, 47)        // Dark gray-blue
#define COLOR_BUTTON_HOVER  RGB(48, 54, 61)        // Slightly lighter
#define COLOR_BUTTON_TEXT   RGB(255, 255, 255)     // White
#define COLOR_TEXT          RGB(36, 41, 47)        // Dark text
#define COLOR_ACCENT        RGB(9, 105, 218)       // Blue accent

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// UI Controls
HWND hHeadingLabel = NULL;
HWND hInputEdit = NULL;
HWND hOutputEdit = NULL;
HWND hButtons[8] = { NULL };

// Custom fonts and brushes
HFONT hHeadingFont = NULL;
HFONT hButtonFont = NULL;
HFONT hEditFont = NULL;
HBRUSH hBrushBgMain = NULL;
HBRUSH hBrushBgPanel = NULL;

// Button labels
const WCHAR* buttonLabels[8] = {
    L"Tokenize", L"Parse", L"Analyze", L"Execute",
    L"Clear", L"Load", L"Save", L"Export"
};

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
void                CreateModernUI(HWND hWnd);
void                ResizeControls(HWND hWnd);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // Initialize common controls
    INITCOMMONCONTROLSEX icex;
    icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icex.dwICC = ICC_STANDARD_CLASSES;
    InitCommonControlsEx(&icex);

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_DOTLANGDESKTOP, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DOTLANGDESKTOP));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    // Cleanup
    if (hHeadingFont) DeleteObject(hHeadingFont);
    if (hButtonFont) DeleteObject(hButtonFont);
    if (hEditFont) DeleteObject(hEditFont);
    if (hBrushBgMain) DeleteObject(hBrushBgMain);
    if (hBrushBgPanel) DeleteObject(hBrushBgPanel);

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DOTLANGDESKTOP));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = CreateSolidBrush(COLOR_BG_MAIN);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_DOTLANGDESKTOP);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   // Create brushes for colors
   hBrushBgMain = CreateSolidBrush(COLOR_BG_MAIN);
   hBrushBgPanel = CreateSolidBrush(COLOR_BG_PANEL);

   HWND hWnd = CreateWindowW(szWindowClass, L"DotLang Lexer", 
      WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, CW_USEDEFAULT, 1200, 700, 
      nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

void CreateModernUI(HWND hWnd)
{
    // Create modern fonts
    hHeadingFont = CreateFont(
        -28, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
        L"Segoe UI"
    );

    hEditFont = CreateFont(
        -16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
        L"Consolas"
    );

    hButtonFont = CreateFont(
        -15, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
        L"Segoe UI"
    );

    // Create heading label
    hHeadingLabel = CreateWindowExW(
        0,
        L"STATIC",
        L"DotLang Lexer",
        WS_CHILD | WS_VISIBLE | SS_LEFT,
        0, 0, 0, 0,
        hWnd,
        (HMENU)IDC_HEADING_LABEL,
        hInst,
        NULL
    );
    SendMessage(hHeadingLabel, WM_SETFONT, (WPARAM)hHeadingFont, TRUE);

    // Create input edit control (left panel)
    hInputEdit = CreateWindowExW(
        WS_EX_CLIENTEDGE,
        L"EDIT",
        L"",
        WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL | ES_WANTRETURN,
        0, 0, 0, 0,
        hWnd,
        (HMENU)IDC_INPUT_EDIT,
        hInst,
        NULL
    );
    SendMessage(hInputEdit, WM_SETFONT, (WPARAM)hEditFont, TRUE);

    // Create output edit control (right panel)
    hOutputEdit = CreateWindowExW(
        WS_EX_CLIENTEDGE,
        L"EDIT",
        L"",
        WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY,
        0, 0, 0, 0,
        hWnd,
        (HMENU)IDC_OUTPUT_EDIT,
        hInst,
        NULL
    );
    SendMessage(hOutputEdit, WM_SETFONT, (WPARAM)hEditFont, TRUE);

    // Create 8 buttons with owner-draw style
    for (int i = 0; i < 8; i++)
    {
        hButtons[i] = CreateWindowExW(
            0,
            L"BUTTON",
            buttonLabels[i],
            WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
            0, 0, 0, 0,
            hWnd,
            (HMENU)(IDC_BUTTON1 + i),
            hInst,
            NULL
        );
        SendMessage(hButtons[i], WM_SETFONT, (WPARAM)hButtonFont, TRUE);
    }

    ResizeControls(hWnd);
}

void ResizeControls(HWND hWnd)
{
    RECT rcClient;
    GetClientRect(hWnd, &rcClient);

    int clientWidth = rcClient.right - rcClient.left;
    int clientHeight = rcClient.bottom - rcClient.top;

    // Layout constants
    const int PADDING = 20;
    const int HEADING_HEIGHT = 50;
    const int HEADING_SPACING = 16;
    const int PANEL_SPACING = 16;
    const int BUTTON_HEIGHT = 40;
    const int BUTTON_SPACING = 12;
    const int BOTTOM_SECTION_HEIGHT = BUTTON_HEIGHT + (PADDING * 2);

    // Position heading
    SetWindowPos(hHeadingLabel, NULL,
        PADDING, PADDING,
        clientWidth - (PADDING * 2), HEADING_HEIGHT,
        SWP_NOZORDER);

    // Calculate panel dimensions
    int panelsStartY = PADDING + HEADING_HEIGHT + HEADING_SPACING;
    int panelWidth = (clientWidth - (PADDING * 3) - PANEL_SPACING) / 2;
    int panelHeight = clientHeight - panelsStartY - BOTTOM_SECTION_HEIGHT - PADDING;

    // Position input panel (left)
    SetWindowPos(hInputEdit, NULL,
        PADDING, panelsStartY,
        panelWidth, panelHeight,
        SWP_NOZORDER);

    // Position output panel (right)
    SetWindowPos(hOutputEdit, NULL,
        PADDING + panelWidth + PANEL_SPACING, panelsStartY,
        panelWidth, panelHeight,
        SWP_NOZORDER);

    // Calculate button layout
    int totalButtonsWidth = clientWidth - (PADDING * 2);
    int buttonWidth = (totalButtonsWidth - (BUTTON_SPACING * 7)) / 8;
    int buttonY = panelsStartY + panelHeight + PADDING;

    // Position buttons
    for (int i = 0; i < 8; i++)
    {
        int buttonX = PADDING + (buttonWidth + BUTTON_SPACING) * i;
        SetWindowPos(hButtons[i], NULL,
            buttonX, buttonY,
            buttonWidth, BUTTON_HEIGHT,
            SWP_NOZORDER);
    }
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        CreateModernUI(hWnd);
        break;

    case WM_SIZE:
        ResizeControls(hWnd);
        break;

    case WM_CTLCOLOREDIT:
        {
            HDC hdcEdit = (HDC)wParam;
            SetTextColor(hdcEdit, COLOR_TEXT);
            SetBkColor(hdcEdit, COLOR_BG_PANEL);
            return (LRESULT)hBrushBgPanel;
        }
        break;

    case WM_CTLCOLORSTATIC:
        {
            HDC hdcStatic = (HDC)wParam;
            HWND hControl = (HWND)lParam;
            
            // Check if it's the heading label
            if (hControl == hHeadingLabel)
            {
                SetTextColor(hdcStatic, COLOR_TEXT);
                SetBkColor(hdcStatic, COLOR_BG_MAIN);
                // Use a custom bold style for heading
                return (LRESULT)hBrushBgMain;
            }
            
            SetTextColor(hdcStatic, COLOR_TEXT);
            SetBkColor(hdcStatic, COLOR_BG_MAIN);
            return (LRESULT)hBrushBgMain;
        }
        break;

    case WM_DRAWITEM:
        {
            LPDRAWITEMSTRUCT pDIS = (LPDRAWITEMSTRUCT)lParam;
            if (pDIS->CtlType == ODT_BUTTON)
            {
                // Custom button drawing for modern look
                HDC hdc = pDIS->hDC;
                RECT rc = pDIS->rcItem;

                // Determine button state
                bool isPressed = (pDIS->itemState & ODS_SELECTED) != 0;
                bool isFocused = (pDIS->itemState & ODS_FOCUS) != 0;
                COLORREF bgColor = (isPressed || isFocused) ? COLOR_BUTTON_HOVER : COLOR_BUTTON_BG;

                // Draw button background with rounded corners
                HBRUSH hBrush = CreateSolidBrush(bgColor);
                HPEN hPen = CreatePen(PS_SOLID, 1, bgColor);
                HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
                HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
                
                // Draw rounded rectangle (6px radius)
                RoundRect(hdc, rc.left, rc.top, rc.right, rc.bottom, 6, 6);
                
                SelectObject(hdc, hOldPen);
                SelectObject(hdc, hOldBrush);
                DeleteObject(hPen);
                DeleteObject(hBrush);

                // Add subtle border for pressed state
                if (isPressed)
                {
                    HPEN hAccentPen = CreatePen(PS_SOLID, 2, COLOR_ACCENT);
                    HPEN hOldAccentPen = (HPEN)SelectObject(hdc, hAccentPen);
                    HBRUSH hOldAccentBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));
                    RoundRect(hdc, rc.left + 1, rc.top + 1, rc.right - 1, rc.bottom - 1, 6, 6);
                    SelectObject(hdc, hOldAccentPen);
                    SelectObject(hdc, hOldAccentBrush);
                    DeleteObject(hAccentPen);
                }

                // Draw button text
                WCHAR buttonText[256];
                GetWindowTextW(pDIS->hwndItem, buttonText, 256);
                
                SetBkMode(hdc, TRANSPARENT);
                SetTextColor(hdc, COLOR_BUTTON_TEXT);
                HFONT hOldFont = (HFONT)SelectObject(hdc, hButtonFont);
                DrawTextW(hdc, buttonText, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                SelectObject(hdc, hOldFont);

                return TRUE;
            }
        }
        break;

    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            case IDC_BUTTON1:  // Tokenize
                SetWindowTextW(hOutputEdit, L"Tokenizing...\r\nFeature coming soon!");
                break;
            case IDC_BUTTON2:  // Parse
                SetWindowTextW(hOutputEdit, L"Parsing...\r\nFeature coming soon!");
                break;
            case IDC_BUTTON3:  // Analyze
                SetWindowTextW(hOutputEdit, L"Analyzing...\r\nFeature coming soon!");
                break;
            case IDC_BUTTON4:  // Execute
                SetWindowTextW(hOutputEdit, L"Executing...\r\nFeature coming soon!");
                break;
            case IDC_BUTTON5:  // Clear
                SetWindowTextW(hInputEdit, L"");
                SetWindowTextW(hOutputEdit, L"");
                break;
            case IDC_BUTTON6:  // Load
                SetWindowTextW(hOutputEdit, L"Load file...\r\nFeature coming soon!");
                break;
            case IDC_BUTTON7:  // Save
                SetWindowTextW(hOutputEdit, L"Save file...\r\nFeature coming soon!");
                break;
            case IDC_BUTTON8:  // Export
                SetWindowTextW(hOutputEdit, L"Export...\r\nFeature coming soon!");
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;

    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // Background is handled by brush
            EndPaint(hWnd, &ps);
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

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
