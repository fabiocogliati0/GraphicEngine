#pragma once

#include "DirectXRenderer.h"

#include <Windows.h>
#include <cassert>

namespace GraphicsEngine
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
			) : mRenderer(nullptr)
		{
			//Create window descriptor
			WNDCLASSEX windowClassDesc;
			windowClassDesc.cbSize = sizeof(WNDCLASSEX);
			windowClassDesc.style = CS_HREDRAW | CS_VREDRAW;
			windowClassDesc.lpfnWndProc = WndProc;
			windowClassDesc.cbClsExtra = 0;
			windowClassDesc.cbWndExtra = 0;
			windowClassDesc.hInstance = iHInstance;
			windowClassDesc.hIcon = LoadIcon(iHInstance, (LPCTSTR)IDI_APPLICATION);
			windowClassDesc.hCursor = LoadCursor(nullptr, IDC_ARROW);
			windowClassDesc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
			windowClassDesc.lpszMenuName = nullptr;
			windowClassDesc.lpszClassName = iWindowClassName;
			windowClassDesc.hIconSm = LoadIcon(iHInstance, (LPCTSTR)IDI_APPLICATION);

			//Class registration
			if (!RegisterClassEx(&windowClassDesc))
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

			//Create renderer
			mRenderer = new DirectXRenderer(mWindowHandler, iMultiSampleCount);

		}

		~Window()
		{
			if (mRenderer != nullptr)
			{
				delete mRenderer;
			}
		}

		void render()
		{
			mRenderer->render();
		}
		
		void addObjectToRender(Object* iObject)
		{
			mRenderer->addObjectToRender(iObject);
		}

		//PENSARE COME FARE A PRENDERE IL DEVICE
		DirectXRenderer* getRender() const
		{
			return mRenderer;
		}

	private :
	
		HWND mWindowHandler;
		DirectXRenderer* mRenderer;

	};

}
