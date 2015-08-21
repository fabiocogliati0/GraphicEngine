#include "MyDirectXWindow.h"

#include <cmath>
#include <cassert>

D3D11_INPUT_ELEMENT_DESC layoutVertex[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

MyDirectXWindow::MyDirectXWindow(
	const HINSTANCE iHInstance,
	int iNCmdShow
	) : DirectXWindow(iHInstance, iNCmdShow, gMultiSampleCount, gWindowTitle, gWindowClassName, gWindowSizeX, gWindowSizeY),
		mAspectRatio(static_cast<float>(gWindowSizeX) / gWindowSizeY)
{
}

int MyDirectXWindow::run()
{

	init();

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
			//Set render target
			mDeviceContext->OMSetRenderTargets(1, &mRenderTargetView, nullptr);

			//clean screen with clean color
			float clearColor[4] = { 1.0f, 0.0f, 0.0f, 1.0f };		//TODO: parametrizzare
			mDeviceContext->ClearRenderTargetView(mRenderTargetView, clearColor);

			mCamera->renderSetup(mDeviceContext);

			//random translation
			for (int i = 0; i < gMaxNumberOfTriangles; ++i)
			{
				//mTriangles[i].translate(static_cast<float>(rand()) / 1000.0f, static_cast<float>(rand()) / 1000.0f, 0.0f);
				mTriangles[i].translate(	(static_cast<float>(rand() % 100) -50) / 10000.0f,
											(static_cast<float>(rand() % 100) -50) / 10000.0f, 0.0f, mDeviceContext);
			}

			for (int i = 0; i < gMaxNumberOfSquares; ++i)
			{
				mSquares[i].translate(	(static_cast<float>(rand() % 100) -50) / 10000.0f,
										(static_cast<float>(rand() % 100) -50) / 10000.0f, 0.0f, mDeviceContext);
				mSquares[i].render(mDeviceContext);
			}

			//rendering opaques

			mDeviceContext->OMSetBlendState(mBlendingStateOff, nullptr, 0xffffffff);

			int i = 0;
			while (i<gMaxNumberOfTriangles && mTriangles[i].isOpaque() && mTriangles[i].isVisible())
			{
				mTriangles[i].render(mDeviceContext);
				++i;
			}

			int j = 0;
			while (j<gMaxNumberOfSquares && mSquares[j].isOpaque() && mSquares[j].isVisible())
			{
				mSquares[j].render(mDeviceContext);
				++j;
			}

			//rendering transparents

			mDeviceContext->OMSetBlendState(mBlendingStateOn, nullptr, 0xffffffff);

			while (i < gMaxNumberOfTriangles && mTriangles[i].isVisible())
			{
				mTriangles[i].render(mDeviceContext);
				++i;
			}

			while (j<gMaxNumberOfSquares && mSquares[j].isVisible())
			{
				mSquares[j].render(mDeviceContext);
				++j;
			}

			//swap buffers
			mSwapChain->Present(0, 0);
		}
	}

	return msg.wParam;
}


void MyDirectXWindow::init()
{

	//Create Window
	//GraphicsEngine::DirectXWindow window(hInstance, nCmdShow, gMultiSampleCount, L"Advanced C++ Project", L"Project", gWindowSizeX, gWindowSizeY);

	//Create Camera
	mCamera = new GraphicsEngine::Camera(0.0f, 0.0f, -20.0f, 0.0f, 0.0f, 1.0f, mAspectRatio);
	mCamera->initializeOnDevice(mDevice);

	//setCamera(camera);

	createBlendingStates();
	createTrinangles();
	createSquares();

	//Run program
	//int executionReturn = window.run();

	//clean								//TODO: non ce n'� bisogno perch� non ho fatto grab, pensare a cosa sarebbe la cosa migliore da fare
	/*material->release();
	material = nullptr;
	vertexShader->release();
	vertexShader = nullptr;
	pixelShader->release();
	pixelShader = nullptr;
	mesh->release();
	mesh = nullptr;*/


	//return executionReturn;
}

void MyDirectXWindow::createTrinangles()
{

	//Create Vertex Shader
	GraphicsEngine::VertexShader* vertexShader =
		new GraphicsEngine::VertexShader(
		L"C:/Users/Fabio/Documents/Visual Studio 2013/Projects/Progetto Advanced C++/Debug/MonoColorVS.cso",	//TODO: path relativo
		layoutVertex,
		layoutVertexSize);

	//Create Pixel Shader
	GraphicsEngine::PixelShader* pixelShader =	//TODO: path relativo
		new GraphicsEngine::PixelShader(L"C:/Users/Fabio/Documents/Visual Studio 2013/Projects/Progetto Advanced C++/Debug/MonoColorPS.cso");


	//Create Materials
	DirectX::XMFLOAT4 opaqueColor(0.0f, 1.0f, 0.0f, 1.0f);
	DirectX::XMFLOAT4 transparentColor(0.0f, 1.0f, 0.0f, 0.5f);

	GraphicsEngine::Material* opaqueMaterial =
		new GraphicsEngine::Material(opaqueColor, vertexShader, pixelShader);

	GraphicsEngine::Material* transparentMaterial =
		new GraphicsEngine::Material(transparentColor, vertexShader, pixelShader);

	//Create Mesh
	GraphicsEngine::Vertex vertices[] =
	{
		{ DirectX::XMFLOAT3(-0.5f, -0.5f, 0.5f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f) },
		{ DirectX::XMFLOAT3(0.0f, 0.5f, 0.5f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f) },
		{ DirectX::XMFLOAT3(0.5f, -0.5f, 0.5f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f) },
	};

	unsigned int indices[] =
	{
		0, 1, 2,
	};

	GraphicsEngine::Mesh* mesh =
		new GraphicsEngine::Mesh(vertices, 3, indices, 3);


	for (int i = 0; i < gMaxNumberOfTriangles; ++i)
	{
		//Create Trasnform
		GraphicsEngine::WorldTransform transform;
		transform.translate((i - (static_cast<int>(gMaxNumberOfTriangles) / 2)) * 1.5f, 1.0f, 0.0f);

		//Create Object
		if (i < gMaxNumberOfTriangles / 2)
		{
			mTriangles[i] = GraphicsEngine::Object(mesh, opaqueMaterial, transform);
		}
		else
		{
			mTriangles[i] = GraphicsEngine::Object(mesh, transparentMaterial, transform);
		}
		mTriangles[i].initializeOnDevice(mDevice);
	}

	//mTriangles[3].setVisible(false); ////

}

void MyDirectXWindow::createSquares()
{

	//Create Vertex Shader
	GraphicsEngine::VertexShader* vertexShader =
		new GraphicsEngine::VertexShader(
		L"C:/Users/Fabio/Documents/Visual Studio 2013/Projects/Progetto Advanced C++/Debug/MonoColorVS.cso",	//TODO: path relativo
		layoutVertex,
		layoutVertexSize);

	//Create Pixel Shader
	GraphicsEngine::PixelShader* pixelShader =	//TODO: path relativo
		new GraphicsEngine::PixelShader(L"C:/Users/Fabio/Documents/Visual Studio 2013/Projects/Progetto Advanced C++/Debug/MonoColorPS.cso");


	//Create Material
	DirectX::XMFLOAT4 color(0.0f, 0.0f, 1.0f, 1.0f);

	GraphicsEngine::Material* material =
		new GraphicsEngine::Material(color, vertexShader, pixelShader);

	//Create Mesh
	GraphicsEngine::Vertex vertices[] =
	{
		{ DirectX::XMFLOAT3(-0.5f, -0.5f, 0.5f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f) },
		{ DirectX::XMFLOAT3(-0.5f, 0.5f, 0.5f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f) },
		{ DirectX::XMFLOAT3(0.5f, -0.5f, 0.5f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f) },
		{ DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f) }
	};

	unsigned int indices[] =
	{
		0, 1, 2,
		1, 3, 2
	};

	GraphicsEngine::Mesh* mesh =
		new GraphicsEngine::Mesh(vertices, 4, indices, 6);


	for (int i = 0; i < gMaxNumberOfSquares; ++i)
	{
		//Create Trasnform
		GraphicsEngine::WorldTransform transform;
		transform.translate((i - (static_cast<int>(gMaxNumberOfSquares) / 2)) * 1.5f, -0.0f, 10.0f);

		//Create Object
		mSquares[i] = GraphicsEngine::Object(mesh, material, transform);
		mSquares[i].initializeOnDevice(mDevice);
	}

}

void MyDirectXWindow::createBlendingStates()
{
	D3D11_BLEND_DESC blendDesc;
	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.IndependentBlendEnable = false;
	blendDesc.RenderTarget[0].BlendEnable = true;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	HRESULT result = mDevice->CreateBlendState(&blendDesc, &mBlendingStateOn);
	assert(SUCCEEDED(result));

	blendDesc.RenderTarget[0].BlendEnable = false;
	result = mDevice->CreateBlendState(&blendDesc, &mBlendingStateOff);
	assert(SUCCEEDED(result));
}