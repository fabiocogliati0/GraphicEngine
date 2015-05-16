#pragma once

#include "DirectXRenderer.h"

#include <Windows.h>
#include <cassert>

namespace GraphicEngine
{

	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		PAINTSTRUCT ps;
		HDC hdc;

		switch (message)
		{
			case WM_PAINT:
			{
							 hdc = BeginPaint(hWnd, &ps);
							 EndPaint(hWnd, &ps);
			} break;

			case WM_DESTROY:
			{
							   PostQuitMessage(0);
			} break;

			default:
			{
					   return DefWindowProc(hWnd, message, wParam, lParam);
			}
		}

		return 0;
	}

	class Window
	{

	public:

		Window(
			const HINSTANCE iHInstance,
			int iNCmdShow,
			unsigned int iMultiSampleCount,
			const LPCWSTR iTitle,
			const LPCWSTR iWindowClassName,
			LONG iWidth,
			LONG iHeight
			)
		{
			//Create window descriptor
			mWindowClass.cbSize = sizeof(WNDCLASSEX);
			mWindowClass.style = CS_HREDRAW | CS_VREDRAW;
			mWindowClass.lpfnWndProc = WndProc;
			mWindowClass.cbClsExtra = 0;
			mWindowClass.cbWndExtra = 0;
			mWindowClass.hInstance = iHInstance;
			mWindowClass.hIcon = LoadIcon(iHInstance, (LPCTSTR)IDI_APPLICATION);
			mWindowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
			mWindowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
			mWindowClass.lpszMenuName = nullptr;
			mWindowClass.lpszClassName = iWindowClassName;
			mWindowClass.hIconSm = LoadIcon(iHInstance, (LPCTSTR)IDI_APPLICATION);

			//Class registration
			if (!RegisterClassEx(&mWindowClass))
				assert(false);

			//Create Window
			mWindowHandler = CreateWindow
			(
				iWindowClassName,
				iTitle,
				WS_OVERLAPPEDWINDOW,
				CW_USEDEFAULT,
				CW_USEDEFAULT,
				iWidth,
				iHeight,
				nullptr,
				nullptr,
				iHInstance,
				nullptr
			);

			if (!mWindowHandler)
				assert(false);

			//show Window
			ShowWindow(mWindowHandler, iNCmdShow);

			//create DirectxRender
			mRenderer = DirectXRenderer(mWindowHandler, iMultiSampleCount);

		}

		void render()
		{
			mRenderer.render();
		}


	private :
	
		WNDCLASSEX mWindowClass;
		HWND mWindowHandler;
		DirectXRenderer mRenderer;

	};

}
