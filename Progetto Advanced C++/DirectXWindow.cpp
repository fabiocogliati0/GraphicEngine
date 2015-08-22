#include "DirectXWindow.h"

#include <Windows.h>
#include <d3d11.h>
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


	DirectXWindow::DirectXWindow(
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


		//Swap chain descriptor
		DXGI_SWAP_CHAIN_DESC swapChainDesc;
		ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
		swapChainDesc.BufferCount = 1;
		swapChainDesc.BufferDesc.Width = iWidth;
		swapChainDesc.BufferDesc.Height = iHeight;
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.OutputWindow = mWindowHandler;
		swapChainDesc.SampleDesc.Count = iMultiSampleCount;
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.Windowed = TRUE;

		//Driver types
		D3D_DRIVER_TYPE driverTypes[] =
		{
			D3D_DRIVER_TYPE_HARDWARE,
			D3D_DRIVER_TYPE_REFERENCE,
			D3D_DRIVER_TYPE_SOFTWARE,
		};

		UINT createDeviceFlags = 0;
#ifdef _DEBUG
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
		UINT numDriverTypes = sizeof(driverTypes) / sizeof(driverTypes[0]);

		//Try to create Device and SwapChain for the first avaible driver type
		HRESULT hr;
		for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; ++driverTypeIndex)
		{
			mDriverType = driverTypes[driverTypeIndex];
			hr = D3D11CreateDeviceAndSwapChain
				(
				nullptr,
				mDriverType,
				nullptr,
				createDeviceFlags,
				nullptr,
				0,
				D3D11_SDK_VERSION,
				&swapChainDesc,
				&mSwapChain,
				&mDevice,
				nullptr,
				&mDeviceContext
				);

			if (SUCCEEDED(hr))
				break;
		}

		assert(SUCCEEDED(hr));

		//Get backBuffer from swap chain
		ID3D11Texture2D* backBuffer;
		hr = mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
		assert(SUCCEEDED(hr));

		//Create render target view
		hr = mDevice->CreateRenderTargetView(backBuffer, nullptr, &mRenderTargetView);
		backBuffer->Release();
		assert(SUCCEEDED(hr));

		//Create Depth and Stencil Buffer
		ID3D11Texture2D* pDepthStencil = nullptr;
		D3D11_TEXTURE2D_DESC descDepth;
		descDepth.Width = iWidth;
		descDepth.Height = iHeight;
		descDepth.MipLevels = 1;
		descDepth.ArraySize = 1;
		descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;;
		descDepth.SampleDesc.Count = iMultiSampleCount;
		descDepth.SampleDesc.Quality = 0;
		descDepth.Usage = D3D11_USAGE_DEFAULT;
		descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		descDepth.CPUAccessFlags = 0;
		descDepth.MiscFlags = 0;

		hr = mDevice->CreateTexture2D(&descDepth, nullptr, &pDepthStencil);
		assert(SUCCEEDED(hr));

		//Create Depth and Stencil view
		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
		descDSV.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		descDSV.ViewDimension = (iMultiSampleCount > 1) ? D3D11_DSV_DIMENSION_TEXTURE2DMS : D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = 0;
		descDSV.Flags = 0;

		//Create the depth stencil view
		hr = mDevice->CreateDepthStencilView(pDepthStencil, &descDSV, &mDepthStencilView);
		assert(SUCCEEDED(hr));

		//Set RenderTarget
		mDeviceContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);

		//Set Viewport
		D3D11_VIEWPORT viewport;
		viewport.Width = static_cast<float>(iWidth);
		viewport.Height = static_cast<float>(iHeight);
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		mDeviceContext->RSSetViewports(1, &viewport);

		//show Window
		ShowWindow(mWindowHandler, iNCmdShow);
	}

	DirectXWindow::~DirectXWindow()
	{
		if (mDevice)
			mDeviceContext->ClearState();

		if (mRenderTargetView)
			mRenderTargetView->Release();

		if (mDepthStencilView)
			mDepthStencilView->Release();

		if (mSwapChain)
			mSwapChain->Release();

		if (mDevice)
			mDevice->Release();
	}

}