#include "MyDirectXWindow.h"

#include "DirectXWindow.h"
#include "Object.h"
#include "Camera.h"
#include "Material.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Mesh.h"
#include "Vertex.h"
#include "WorldTransform.h"
#include <d3d11.h>
#include <cmath>
#include <cassert>

//static arrays definitions

static const		LPCWSTR sWindowTitleDefinition						=	L"Advanced C++ Project";
static const		LPCWSTR sWindowClassNameDefinition					=	L"Project";
static const		D3D11_INPUT_ELEMENT_DESC sLayoutVertexDefinition[]	=
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
const LPCWSTR MyDirectXWindow::sWindowTitle = sWindowTitleDefinition;
const LPCWSTR MyDirectXWindow::sWindowClassName = sWindowClassNameDefinition;
const D3D11_INPUT_ELEMENT_DESC* MyDirectXWindow::sLayoutVertex = sLayoutVertexDefinition;


//class definition

MyDirectXWindow::MyDirectXWindow(
	const HINSTANCE iHInstance,
	int iNCmdShow
	) : DirectXWindow(iHInstance, iNCmdShow, sMultiSampleCount, sWindowTitle, sWindowClassName, sWindowSizeX, sWindowSizeY),
		mCamera(nullptr)
{
}

MyDirectXWindow::~MyDirectXWindow()
{
	if (mDepthStateOff)
		mDepthStateOff->Release();

	if (mDepthStateOn)
		mDepthStateOn->Release();

	if (mRasterizerStateBackFaceCulling)
		mRasterizerStateBackFaceCulling->Release();

	if (mBlendingStateOff)
		mBlendingStateOff->Release();

	if (mBlendingStateOn)
		mBlendingStateOn->Release();
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
			//clean render target
			float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
			mDeviceContext->ClearRenderTargetView(mRenderTargetView, clearColor);

			//clean depthStencil Buffer
			mDeviceContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

			//setup Camera
			mCamera->renderSetup(mDeviceContext);

			//randomic Translation to each object
			moveObjects();

			//render Objects
			renderObjects();

			//swap buffers
			mSwapChain->Present(0, 0);
		}
	}

	return msg.wParam;
}

void MyDirectXWindow::init()
{
	createDepthStencilState();
	createRasterizerStates();
	createBlendingStates();
	createCamera();
	createTrinangles();
	createSquares();
}

void MyDirectXWindow::moveObjects()
{
	for (int i = 0; i < sMaxNumberOfTriangles; ++i)
	{
		mTriangles[i].translate((static_cast<float>(rand() % 100) - 50) / 10000.0f,
			(static_cast<float>(rand() % 100) - 50) / 10000.0f, 0.0f, mDeviceContext);
	}

	for (int i = 0; i < sMaxNumberOfSquares; ++i)
	{
		mSquares[i].translate((static_cast<float>(rand() % 100) - 50) / 10000.0f,
			(static_cast<float>(rand() % 100) - 50) / 10000.0f, 0.0f, mDeviceContext);
	}
}

void MyDirectXWindow::renderObjects()
{

	mDeviceContext->RSSetState(mRasterizerStateBackFaceCulling);

	//rendering opaques
	mDeviceContext->OMSetDepthStencilState(mDepthStateOn, 0);
	mDeviceContext->OMSetBlendState(mBlendingStateOff, nullptr, 0xffffffff);

	int i = 0;
	while (i<sMaxNumberOfTriangles && mTriangles[i].isOpaque() && mTriangles[i].isVisible())
	{
		mTriangles[i].render(mDeviceContext);
		++i;
	}

	int j = 0;
	while (j<sMaxNumberOfSquares && mSquares[j].isOpaque() && mSquares[j].isVisible())
	{
		mSquares[j].render(mDeviceContext);
		++j;
	}

	//rendering transparents
	mDeviceContext->OMSetDepthStencilState(mDepthStateOff, 0);
	mDeviceContext->OMSetBlendState(mBlendingStateOn, nullptr, 0xffffffff);

	while (i < sMaxNumberOfTriangles && mTriangles[i].isVisible())
	{
		mTriangles[i].render(mDeviceContext);
		++i;
	}

	while (j<sMaxNumberOfSquares && mSquares[j].isVisible())
	{
		mSquares[j].render(mDeviceContext);
		++j;
	}
}

void MyDirectXWindow::createCamera()
{
	mCamera = new GraphicsEngine::Camera(0.0f, 0.0f, -20.0f, 0.0f, 0.0f, 1.0f, static_cast<float>(sWindowSizeX) / sWindowSizeY);
	mCamera->initializeOnDevice(mDevice);
}

//TODO: Path relativo ai file degli shader
void MyDirectXWindow::createTrinangles()
{

	//Create Vertex Shader
	GraphicsEngine::VertexShader* vertexShader =
		new GraphicsEngine::VertexShader(
		L"C:/Users/Fabio/Documents/Visual Studio 2013/Projects/Progetto Advanced C++/Debug/MonoColorVS.cso",	//TODO: path relativo
		sLayoutVertex,
		sLayoutVertexSize);

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

	//Create Triangle Mesh
	GraphicsEngine::Vertex vertices[] =
	{
		{ DirectX::XMFLOAT3(-0.5f, -0.5f, 0.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f) },
		{ DirectX::XMFLOAT3(0.0f, 0.5f, 0.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f) },
		{ DirectX::XMFLOAT3(0.5f, -0.5f, 0.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f) },
	};

	unsigned int indices[] =
	{
		0, 1, 2,
	};

	GraphicsEngine::Mesh* mesh =
		new GraphicsEngine::Mesh(vertices, 3, indices, 3);


	for (int i = 0; i < sMaxNumberOfTriangles; ++i)
	{
		//Create Trasnform
		GraphicsEngine::WorldTransform transform;
		transform.translate((i - (static_cast<int>(sMaxNumberOfTriangles) / 2)) * 1.5f, 1.0f, 0.0f);

		//Create Object
		if (i < sMaxNumberOfTriangles / 2)
		{
			mTriangles[i] = GraphicsEngine::Object(mesh, opaqueMaterial, transform);
		}
		else
		{
			mTriangles[i] = GraphicsEngine::Object(mesh, transparentMaterial, transform);
		}
		mTriangles[i].initializeOnDevice(mDevice);
	}

}

//TODO: Path relativo ai file degli shader
void MyDirectXWindow::createSquares()
{

	//Create Vertex Shader
	GraphicsEngine::VertexShader* vertexShader =
		new GraphicsEngine::VertexShader(
		L"C:/Users/Fabio/Documents/Visual Studio 2013/Projects/Progetto Advanced C++/Debug/MonoColorVS.cso",	//TODO: path relativo
		sLayoutVertex,
		sLayoutVertexSize);

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
		{ DirectX::XMFLOAT3(-0.5f, -0.5f, 0.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f) },
		{ DirectX::XMFLOAT3(-0.5f, 0.5f, 0.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f) },
		{ DirectX::XMFLOAT3(0.5f, -0.5f, 0.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f) },
		{ DirectX::XMFLOAT3(0.5f, 0.5f, 0.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f) }
	};

	unsigned int indices[] =
	{
		0, 1, 2,
		1, 3, 2
	};

	GraphicsEngine::Mesh* mesh =
		new GraphicsEngine::Mesh(vertices, 4, indices, 6);


	for (int i = 0; i < sMaxNumberOfSquares; ++i)
	{
		//Create Trasnform
		GraphicsEngine::WorldTransform transform;
		transform.translate((i - (static_cast<int>(sMaxNumberOfSquares) / 2)) * 1.5f, 1.0f, 5.0f);

		//Create Object
		mSquares[i] = GraphicsEngine::Object(mesh, material, transform);
		mSquares[i].initializeOnDevice(mDevice);
	}

}

void MyDirectXWindow::createDepthStencilState()
{
	D3D11_DEPTH_STENCIL_DESC dsDesc;
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
	dsDesc.StencilEnable = false;

	HRESULT result = mDevice->CreateDepthStencilState(&dsDesc, &mDepthStateOn);
	assert(SUCCEEDED(result));

	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	result = mDevice->CreateDepthStencilState(&dsDesc, &mDepthStateOff);
	assert(SUCCEEDED(result));
}

void MyDirectXWindow::createRasterizerStates()
{
	D3D11_RASTERIZER_DESC rsDesc;
	rsDesc.CullMode = D3D11_CULL_FRONT;
	rsDesc.FillMode = D3D11_FILL_SOLID;
	rsDesc.FrontCounterClockwise = true;
	rsDesc.DepthBias = false;
	rsDesc.DepthBiasClamp = 0;
	rsDesc.SlopeScaledDepthBias = 0;
	rsDesc.DepthClipEnable = true;
	rsDesc.ScissorEnable = false;
	rsDesc.MultisampleEnable = false;

	HRESULT result = mDevice->CreateRasterizerState(&rsDesc, &mRasterizerStateBackFaceCulling);
	assert(SUCCEEDED(result));
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