#pragma once

#include <Windows.h>
#include <d3d11.h>
#include <cassert>

namespace GraphicsEngine
{

	class DirectXRenderer
	{

	public:

		DirectXRenderer()
			: mDriverType(D3D_DRIVER_TYPE_NULL)
		{
			//TODO: ragionare su cosa altro metterci
		}

		DirectXRenderer(HWND iWindowHandler, const int iMultiSampleCount)
		{
			RECT windowRect;
			GetClientRect(iWindowHandler, &windowRect);
			UINT width = windowRect.right - windowRect.left;
			UINT height = windowRect.bottom - windowRect.top;

			UINT createDeviceFlags = 0;
#ifdef _DEBUG
			createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

			//Swap chain descriptor
			DXGI_SWAP_CHAIN_DESC swapChainDesc;
			ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
			swapChainDesc.BufferCount = 1;							//TODO pensare se parametrizzare
			swapChainDesc.BufferDesc.Width = width;
			swapChainDesc.BufferDesc.Height = height;
			swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;	//TODO pensare se parametrizzare
			swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
			swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			swapChainDesc.OutputWindow = iWindowHandler;
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
			UINT numDriverTypes = sizeof(driverTypes) / sizeof(driverTypes[0]);

			//Try to create Device and SwapChain for the first driver type avaibles
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

			assert(!FAILED(hr));


			//Get backBuffer from swap chain
			ID3D11Texture2D* backBuffer;
			hr = mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
			assert(!FAILED(hr));

			//Create and set render target
			hr = mDevice->CreateRenderTargetView(backBuffer, nullptr, &mRenderTargetView);
			backBuffer->Release();
			assert(!FAILED(hr));
			mDeviceContext->OMSetRenderTargets(1, &mRenderTargetView, nullptr);

			//Set Viewport
			D3D11_VIEWPORT viewport;
			viewport.Width = static_cast<float>(width);
			viewport.Height = static_cast<float>(height);
			viewport.MinDepth = 0.0f;
			viewport.MaxDepth = 1.0f;
			viewport.TopLeftX = 0;
			viewport.TopLeftY = 0;
			mDeviceContext->RSSetViewports(1, &viewport);
		}

		~DirectXRenderer()
		{
			cleanupDevice();
		}

		void cleanupDevice()
		{
			if (mDevice)
				mDeviceContext->ClearState();

			if (mRenderTargetView)
				mRenderTargetView->Release();

			if (mSwapChain)
				mSwapChain->Release();

			if (mDevice)
				mDevice->Release();
		}

		void render()
		{
			//TODO: mettere delle assert per verificare che si possa eseguire una render
			//TODO: ovviamente fa cose più complicate, continuare

			//clean screen with clean color
			float clearColor[4] = { 0.39f, 0.58f, 0.93f, 1.0f };		//TODO: parametrizzare
			mDeviceContext->ClearRenderTargetView(mRenderTargetView, clearColor);
			
			//swap buffers
			mSwapChain->Present(0, 0);
		}

		ID3D11Device* getDevice() const
		{
			return mDevice;
		}

		ID3D11DeviceContext* getDeviceContext() const
		{
			return mDeviceContext;
		}

	private:

		D3D_DRIVER_TYPE mDriverType;
		IDXGISwapChain* mSwapChain;
		ID3D11Device* mDevice;
		ID3D11DeviceContext* mDeviceContext;
		ID3D11RenderTargetView* mRenderTargetView;

	};

}

