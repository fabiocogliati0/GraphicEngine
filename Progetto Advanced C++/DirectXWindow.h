#pragma once

#include "Object.h"
#include "Camera.h"

#include <Windows.h>
#include <d3d11.h>
#include <cassert>

namespace GraphicsEngine
{

	class DirectXWindow
	{

	public:

		DirectXWindow(
			const HINSTANCE iHInstance,
			int iNCmdShow,
			unsigned int iMultiSampleCount,
			const LPCWSTR iTitle,
			const LPCWSTR iWindowClassName,
			LONG iWidth,
			LONG iHeight);

		~DirectXWindow();

		virtual int run() = 0;


	protected:

		ID3D11DeviceContext* mDeviceContext;
		D3D_DRIVER_TYPE mDriverType;
		IDXGISwapChain* mSwapChain;
		ID3D11Device* mDevice;
		ID3D11RenderTargetView* mRenderTargetView;
		ID3D11DepthStencilView* mDepthStencilView;

	private:

		HWND mWindowHandler;

	};

}
