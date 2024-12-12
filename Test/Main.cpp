#include <iostream>

#include <Windows.h>

int32_t DisplayWidth;
int32_t DisplayHeight;

int32_t ClientWidth;
int32_t ClientHeight;

uint8_t Running = 1;

LRESULT CALLBACK WindowProcedure(HWND HWindow, UINT Message, WPARAM UserParameter, LPARAM DataParameter);
void DrawPixel(void *Display, int32_t X, int32_t Y, uint32_t Color, int32_t DisplayWidth, int32_t DisplayHeight);
void FillRectangle(void *Display, int32_t X, int32_t Y, int32_t Width, int32_t Height, uint32_t Color, int32_t DisplayWidth, int32_t DisplayHeight);

int WINAPI WinMain(HINSTANCE HInstance, HINSTANCE HPreviousInstance, LPSTR Command, int CommandLength)
{


    return 0;
    WNDCLASSW WindowClass = { 0 };
    WindowClass.hbrBackground = (HBRUSH) COLOR_WINDOW;
    WindowClass.hInstance = HInstance;
    WindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    WindowClass.lpszClassName = L"Root";
    WindowClass.lpfnWndProc = WindowProcedure;

    if (!RegisterClassW(&WindowClass)) return -1;

    int32_t Width = 800;
    int32_t Height = 800;

    RECT WindowRectangle = { 0 };
    WindowRectangle.right = Width;
    WindowRectangle.bottom = Height;
    WindowRectangle.left = 0;
    WindowRectangle.top = 0;

    AdjustWindowRect(&WindowRectangle, WS_OVERLAPPEDWINDOW | WS_VISIBLE, 0);

    HWND window = CreateWindowW(
        WindowClass.lpszClassName,
        L"GIF Loader Test",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        WindowRectangle.right - WindowRectangle.left,
        WindowRectangle.bottom - WindowRectangle.top,
        NULL,
        NULL,
        NULL,
        NULL
    );

    GetWindowRect(window, &WindowRectangle);
    ClientWidth = WindowRectangle.right - WindowRectangle.left;
    ClientHeight = WindowRectangle.bottom - WindowRectangle.top;

    DisplayWidth = Width;
    DisplayHeight = Height;

    uint32_t BytesPerPixel = 4;

    void *Display = VirtualAlloc(
        0,
        DisplayWidth * DisplayHeight * BytesPerPixel,
        MEM_RESERVE | MEM_COMMIT,
        PAGE_READWRITE
    );

    if (!Display) {
        printf("Failed to allocate display buffer!\n");
        return 1;
    }

    BITMAPINFO BitmapInfo;
    BitmapInfo.bmiHeader.biSize = sizeof(BitmapInfo.bmiHeader);
    BitmapInfo.bmiHeader.biWidth = DisplayWidth;
    BitmapInfo.bmiHeader.biHeight = -DisplayHeight;
    BitmapInfo.bmiHeader.biPlanes = 1;
    BitmapInfo.bmiHeader.biBitCount = 32;
    BitmapInfo.bmiHeader.biCompression = BI_RGB;

    HDC HDisplayContext = GetDC(window);

    while (Running) {
        MSG Msg;
        while (PeekMessageW(&Msg, NULL, 0, 0, PM_REMOVE)) {
            if (Msg.message == WM_QUIT) Running = 0;
            TranslateMessage(&Msg);
            DispatchMessageW(&Msg);
        }


        FillRectangle(Display, 0, 0, 200, 200, 0x0000FF00, DisplayWidth, DisplayHeight);


        StretchDIBits(
            HDisplayContext, 0, 0,
            DisplayWidth, DisplayHeight,
            0, 0,
            DisplayWidth, DisplayHeight,
            Display, &BitmapInfo,
            DIB_RGB_COLORS,
            SRCCOPY
        );
    }

    VirtualFree(Display, DisplayWidth * DisplayHeight * BytesPerPixel, MEM_RELEASE);
    return 0;
}

LRESULT CALLBACK WindowProcedure(HWND HWindow, UINT Message, WPARAM UserParameter, LPARAM DataParameter)
{
    switch (Message) {
    case WM_DESTROY: {
        PostQuitMessage(0);
        break;
    }
    default: {
        return DefWindowProcW(HWindow, Message, UserParameter, DataParameter);
    }
    }
}

void DrawPixel(void *Display, int32_t X, int32_t Y, uint32_t Color, int32_t DisplayWidth, int32_t DisplayHeight)
{
    if ((X >= 0 && X < DisplayWidth) && (Y >= 0 && Y < DisplayHeight)) {
        *((uint32_t *) Display + (X + Y * DisplayWidth)) = Color;
    }
}

void FillRectangle(void *Display, int32_t X, int32_t Y, int32_t Width, int32_t Height, uint32_t Color, int32_t DisplayWidth, int32_t DisplayHeight)
{
    for (int32_t j = 0; j < Height; ++j) {
        for (int32_t i = 0; i < Width; ++i) {
            DrawPixel(Display, X + i, Y + j , Color, DisplayWidth, DisplayHeight);
        }
    }
}