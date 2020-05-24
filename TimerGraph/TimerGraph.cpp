#define _CRT_SECURE_NO_WARNINGS
#include "pch.h"

#include "CQueue.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
HINSTANCE g_hInstance;
LPCWSTR lpszClass = L"Graph";

CQueue<int> g_Que;

int g_iHeight;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrecInstance,
    LPSTR lpszCmdParam, int nCmdShow)
{
    HWND hWnd;
    MSG Message;
    WNDCLASS wndClass;

    // 클래스 설정
    wndClass.cbClsExtra = 0;
    wndClass.cbWndExtra = 0;
    wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wndClass.hCursor = LoadCursorW(NULL, IDC_ARROW);
    wndClass.hIcon = LoadIconW(NULL, IDI_APPLICATION);
    wndClass.hInstance = hInstance;
    wndClass.lpfnWndProc = (WNDPROC)WndProc;
    wndClass.lpszClassName = lpszClass;
    wndClass.lpszMenuName = NULL;
    wndClass.style = CS_HREDRAW | CS_VREDRAW;

    // 윈도우 클래스 등록
    RegisterClassW(&wndClass);

    // CreateWindow
    hWnd = CreateWindowW
    (
        lpszClass,
        L"Timer Graph",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        NULL,
        (HMENU)NULL,
        hInstance,
        NULL
    );

    // 만들어진 윈도우를 화면에 출력
    ShowWindow(hWnd, nCmdShow);

    // 메시지 루프
    while (GetMessageW(&Message, NULL, 0, 0))
    {
        TranslateMessage(&Message);
        DispatchMessageW(&Message);
    }

    return (int)Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;

    HPEN hPen;
    HPEN hOldPen;

    int iOldX;
    int iOldY;
    int iCurX;
    int iCurY;

    switch (iMessage)
    {
    case WM_CREATE:
        SetTimer(hWnd, 1, 1, NULL);

        g_iHeight = 180;

       // srand(time(NULL));

        break;
    case WM_TIMER:
        // 클라이언트 영역
        InvalidateRect(hWnd, NULL, TRUE);

        if (!g_Que.EnQueue(rand() % 200 + g_iHeight))
        {
            g_Que.DeQueue();
        }

        break;

    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);

        iOldX = 0;
        iOldY = g_Que.GetFrontValue();
        iCurX = iOldX;
        iCurY = iOldY;

        hPen = CreatePen(PS_SOLID, 3, RGB(255, 0, 255));
        hOldPen = (HPEN)SelectObject(hdc, hPen);

        g_Que.InitTemp();
        while (g_Que.Peek(&iOldY))
        {
            MoveToEx(hdc, iCurX * 10, iCurY, NULL);
            LineTo(hdc, iOldX * 10, iOldY);

            iCurX = iOldX;
            iCurY = iOldY;

            iOldX++;
        }
        DeleteObject(SelectObject(hdc, hOldPen));

        EndPaint(hWnd, &ps);
        break;
    case WM_DESTROY:
        KillTimer(hWnd, 1);
        PostQuitMessage(0);
        return 0;
    default:
        DefWindowProcW(hWnd, iMessage, wParam, lParam);
        break;
    }

    return  DefWindowProcW(hWnd, iMessage, wParam, lParam);
}