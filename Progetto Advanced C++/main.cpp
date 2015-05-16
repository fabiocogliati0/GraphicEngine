#include "Window.h"

#include <Windows.h>

const int gMultiSampleCount = 1;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	
	GraphicEngine::Window window(hInstance, nCmdShow, gMultiSampleCount, L"Advanced C++ Project", L"Project", 600L, 600L);

	// Main message loop
	MSG msg = { 0 };
	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			window.render();
		}
	}

	return static_cast<int>(msg.wParam);

}