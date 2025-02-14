#include <iostream>
#include "GifProcessor.h"

#include <Windows.h>

int32_t DisplayWidth;
int32_t DisplayHeight;

int32_t ClientWidth;
int32_t ClientHeight;

uint8_t Running = 1;

LRESULT CALLBACK WindowProcedure(HWND HWindow, UINT Message, WPARAM UserParameter, LPARAM DataParameter);
void DrawPixel(void *Display, int32_t X, int32_t Y, uint32_t Color, int32_t DisplayWidth, int32_t DisplayHeight);
void FillRectangle(void *Display, int32_t X, int32_t Y, int32_t Width, int32_t Height, uint32_t Color, int32_t DisplayWidth, int32_t DisplayHeight);
void DrawImage(void *Display, int32_t X, int32_t Y, SImage *Image, int32_t DisplayWidth, int32_t DisplayHeight);

//int APIENTRY WinMain(HINSTANCE HInstance, HINSTANCE HPreviousInstance, LPSTR Command, int CommandLength)
int main(void)
{
    SGif Gif = { 0 };
    SGif Gif1 = { 0 };
    SGif Gif2 = { 0 };
    SGif Gif3 = { 0 };
    SAnimation GifAnimation = CGifProcessor::Load("Assets\\Textures\\toothless.gif", &Gif);
    SAnimation GifAnimation1 = CGifProcessor::Load("Assets\\Textures\\dance.gif", &Gif1);
    SAnimation GifAnimation2 = CGifProcessor::Load("Assets\\Textures\\mandelbrot.gif", &Gif2);
    SAnimation GifAnimation3 = CGifProcessor::Load("Assets\\Textures\\cubes.gif", &Gif3);

    WNDCLASSW WindowClass = { 0 };
    WindowClass.hbrBackground = (HBRUSH) COLOR_WINDOW;
    WindowClass.hInstance = GetModuleHandleW(NULL);
    WindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    WindowClass.lpszClassName = L"Root";
    WindowClass.lpfnWndProc = WindowProcedure;

    if (!RegisterClassW(&WindowClass)) return -1;

    int32_t Width = 1600;
    int32_t Height = 900;

    RECT WindowRectangle = { 0 };
    WindowRectangle.right = Width;
    WindowRectangle.bottom = Height;
    WindowRectangle.left = 0;
    WindowRectangle.top = 0;

    AdjustWindowRect(&WindowRectangle, WS_OVERLAPPEDWINDOW | WS_VISIBLE, 0);

    HWND window = CreateWindowW(
        WindowClass.lpszClassName,
        L"Pixlo Media Loader Test",
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

    uint32_t AnimationIndex = 0;
    uint32_t AnimationIndex1 = 0;
    uint32_t AnimationIndex2 = 0;
    uint32_t AnimationIndex3 = 0;

    LARGE_INTEGER Frequency, LastTime, CurrentTime;
    QueryPerformanceFrequency(&Frequency);
    QueryPerformanceCounter(&LastTime);

    float DeltaSum = 0;
    float DeltaSum1 = 0;
    float DeltaSum2 = 0;
    float DeltaSum3 = 0;
    float DeltaMax = GifAnimation.Delay / 150.0f;
    float DeltaMax1 = GifAnimation1.Delay / 150.0f;
    float DeltaMax2 = GifAnimation2.Delay / 150.0f;
    float DeltaMax3 = GifAnimation3.Delay / 150.0f;

    while (Running) {
        MSG Msg;
        while (PeekMessageW(&Msg, NULL, 0, 0, PM_REMOVE)) {
            if (Msg.message == WM_QUIT) Running = 0;
            TranslateMessage(&Msg);
            DispatchMessageW(&Msg);
        }

        QueryPerformanceCounter(&CurrentTime);
        double DeltaTime = (double) (CurrentTime.QuadPart - LastTime.QuadPart) / (double) Frequency.QuadPart;
        LastTime = CurrentTime;

        if (DeltaSum >= DeltaMax) {
            AnimationIndex = (AnimationIndex + 1) % (GifAnimation.Size);
            DeltaSum = 0;
        }

        if (DeltaSum1 >= DeltaMax1) {
            AnimationIndex1 = (AnimationIndex1 + 1) % (GifAnimation1.Size);
            DeltaSum1 = 0;
        }

        if (DeltaSum2 >= DeltaMax2) {
            AnimationIndex2 = (AnimationIndex2 + 1) % (GifAnimation2.Size);
            DeltaSum2 = 0;
        }

        if (DeltaSum3 >= DeltaMax3) {
            AnimationIndex3 = (AnimationIndex3 + 1) % (GifAnimation3.Size);
            DeltaSum3 = 0;
        }

        DrawImage(Display, 600, 0, &GifAnimation.Frames[AnimationIndex], DisplayWidth, DisplayHeight);

        DrawImage(Display, 0, 0, &GifAnimation1.Frames[AnimationIndex1], DisplayWidth, DisplayHeight);

        DrawImage(Display, 150, 650, &GifAnimation2.Frames[AnimationIndex2], DisplayWidth, DisplayHeight);

        DrawImage(Display, 600, 360, &GifAnimation3.Frames[AnimationIndex3], DisplayWidth, DisplayHeight);

        StretchDIBits(
            HDisplayContext, 0, 0,
            DisplayWidth, DisplayHeight,
            0, 0,
            DisplayWidth, DisplayHeight,
            Display, &BitmapInfo,
            DIB_RGB_COLORS,
            SRCCOPY
        );
        DeltaSum += (float) DeltaTime;
        DeltaSum1 += (float)DeltaTime;
        DeltaSum2 += (float)DeltaTime;
        DeltaSum3 += (float)DeltaTime;
    }

    VirtualFree(Display, (SIZE_T) (DisplayWidth * DisplayHeight * BytesPerPixel), MEM_RELEASE);
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
    return 1;
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

void DrawImage(void *Display, int32_t X, int32_t Y, SImage *Image, int32_t DisplayWidth, int32_t DisplayHeight)
{
    for (uint32_t j = 0; j < Image->Height; ++j) {
        for (uint32_t i = 0; i < Image->Width; ++i) {
            if ((i >= 0 && i < Image->Width) && (j >= 0 && j < Image->Height)) {
                DrawPixel(Display, X + i, Y + j, ((uint32_t *) Image->Data)[i + j * Image->Width], DisplayWidth, DisplayHeight);
            }
        }
    }
}