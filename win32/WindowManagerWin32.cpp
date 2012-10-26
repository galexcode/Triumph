#include "WindowManager.h"

#if SOL_PLATFORM == SOL_PLATFORM_WIN32
#include "windows.h"
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProcW(hWnd, msg, wParam, lParam);
}
#endif

WindowManager::WindowManager()
{
	mNumWindows = 0;
}

Window* WindowManager::newWindow(int width, int height)
{
	if (mNumWindows < MAX_WINDOWS)
	{
		 return new Window(width, height, ++mNumWindows);
	}
	return 0;
}

Window::Window(int width, int height, int id)
{
	mWidth = width;
	mHeight = height;
	mId = id;

#if SOL_PLATFORM == SOL_PLATFORM_WIN32

	MSG msg;
	HWND hwnd;
	WNDCLASSW wc;
	HINSTANCE hInstance = GetModuleHandle(NULL);

	wc.style         = CS_HREDRAW | CS_VREDRAW;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.lpszClassName = L"Window";
	wc.hInstance     = hInstance;
	wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
	wc.lpszMenuName  = NULL;
	wc.lpfnWndProc   = WndProc;
	wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassW(&wc);
	hwnd = CreateWindowW(wc.lpszClassName, L"Window", 
		WS_OVERLAPPEDWINDOW | WS_VISIBLE, 
		100, 100, mWidth, mHeight, NULL, NULL, hInstance, NULL);

	ShowWindow(hwnd, 1);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		DispatchMessage(&msg);
	}

#endif

}