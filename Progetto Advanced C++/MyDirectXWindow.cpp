#include "MyDirectXWindow.h"

#include "DirectXWindow.h"
#include "Object.h"
#include "AABB.h"
#include "Camera.h"
#include "Material.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Mesh.h"
#include "Vertex.h"
#include "WorldTransform.h"
#include "Frustum.h"

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

MyDirectXWindow::MyDirectXWindow( const HINSTANCE iHInstance, int iNCmdShow) : 
	DirectXWindow(iHInstance, iNCmdShow, sMultiSampleCount, sWindowTitle, sWindowClassName, sWindowSizeX, sWindowSizeY),
	mCamera(GraphicsEngine::Camera(0.0f, 0.0f, -20.0f, 0.0f, 0.0f, 1.0f, static_cast<float>(sWindowSizeX) / sWindowSizeY))
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

	delete[] mObjectsTransform;
	delete[] mObjects;
	delete[] mAABBs;

	delete mVertexShader;
	delete mPixelShader;
	delete mOpaqueMaterialTriangles;
	delete mTransparentMaterialTriangles;
	delete mOpaqueMaterialSquares;
	delete mMeshTriangles;
	delete mMeshSquares;
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
			mCamera.renderSetup(mDeviceContext);

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
	mCamera.initializeOnDevice(mDevice);
	createDepthStencilState();
	createRasterizerStates();
	createBlendingStates();
	createObjects();
}

void MyDirectXWindow::moveObjects()
{
	for (int i = 0; i < sMaxNumberOfTriangles + sMaxNumberOfSquares; ++i)
	{
		mObjectsTransform[i].translate((static_cast<float>(rand() % 100) - 50) / 10000.0f,
			(static_cast<float>(rand() % 100) - 50) / 10000.0f, 0.0f, mDeviceContext);
	}
}

void MyDirectXWindow::renderObjects()
{

	mDeviceContext->RSSetState(mRasterizerStateBackFaceCulling);

	//render opaques
	mDeviceContext->OMSetDepthStencilState(mDepthStateOn, 0);
	mDeviceContext->OMSetBlendState(mBlendingStateOff, nullptr, 0xffffffff);

	const GraphicsEngine::Frustum& frustum = mCamera.getFrustum();

	int i;
	for (i = 0; i < mFirstTransparentIndex; ++i)
	{
		if (!frustum.CheckAABB(mAABBs[i], mObjectsTransform[i]))
		{
			mObjectsTransform[i].renderSetup(mDeviceContext);
			mObjects[i].render(mDeviceContext);
		}
	}

	//render transparents
	mDeviceContext->OMSetDepthStencilState(mDepthStateOff, 0);
	mDeviceContext->OMSetBlendState(mBlendingStateOn, nullptr, 0xffffffff);

	for (; i < mFirstInactiveIndex; ++i)
	{
		if (!frustum.CheckAABB(mAABBs[i], mObjectsTransform[i]))
		{
			mObjectsTransform[i].renderSetup(mDeviceContext);
			mObjects[i].render(mDeviceContext);
		}
	}
}

void MyDirectXWindow::createObjects()
{

	//TODO: brutto così, sovrascrivere l'operatore new di WorldTransform
	void* ptr = _aligned_malloc(sizeof(GraphicsEngine::WorldTransform) * (sMaxNumberOfTriangles + sMaxNumberOfSquares), 16);
	mObjectsTransform = new(ptr) GraphicsEngine::WorldTransform[sMaxNumberOfTriangles + sMaxNumberOfSquares];
	mObjects = new GraphicsEngine::Object[sMaxNumberOfTriangles + sMaxNumberOfSquares];
	mAABBs = new GraphicsEngine::AABB[sMaxNumberOfTriangles + sMaxNumberOfSquares];

	//Create Vertex Shader
	mVertexShader =
		new GraphicsEngine::VertexShader(
		L"./MonoColorVS.cso",
		sLayoutVertex,
		sLayoutVertexSize);

	//Create Pixel Shader
	mPixelShader = new GraphicsEngine::PixelShader(L"./MonoColorPS.cso");


	//Create Materials
	DirectX::XMFLOAT4 opaqueColorTriangles(0.0f, 1.0f, 0.0f, 1.0f);
	DirectX::XMFLOAT4 transparentColorTriangles(0.0f, 1.0f, 0.0f, 0.5f);
	DirectX::XMFLOAT4 opaqueColorSquares(0.0f, 0.0f, 1.0f, 1.0f);

	mOpaqueMaterialTriangles =
		new GraphicsEngine::Material(opaqueColorTriangles, mVertexShader, mPixelShader);

	mTransparentMaterialTriangles =
		new GraphicsEngine::Material(transparentColorTriangles, mVertexShader, mPixelShader);

	mOpaqueMaterialSquares =
		new GraphicsEngine::Material(opaqueColorSquares, mVertexShader, mPixelShader);

	//Create Meshes
	int numberOfVerticesTriangles = 3;
	GraphicsEngine::Vertex verticesTriangles[] =
	{
		{ DirectX::XMFLOAT3(-0.5f, -0.5f, 0.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f) },
		{ DirectX::XMFLOAT3(0.0f, 0.5f, 0.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f) },
		{ DirectX::XMFLOAT3(0.5f, -0.5f, 0.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f) },
	};

	int numberOfIindicesTriangles = 3;
	unsigned int indicesTriangles[] =
	{
		0, 1, 2,
	};

	int numberOfVerticesSquares = 4;
	GraphicsEngine::Vertex verticesSquares[] =
	{
		{ DirectX::XMFLOAT3(-0.5f, -0.5f, 0.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f) },
		{ DirectX::XMFLOAT3(-0.5f, 0.5f, 0.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f) },
		{ DirectX::XMFLOAT3(0.5f, -0.5f, 0.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f) },
		{ DirectX::XMFLOAT3(0.5f, 0.5f, 0.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f) }
	};

	int numberOfIindicesSquares = 6;
	unsigned int indicesSquares[] =
	{
		0, 1, 2,
		1, 3, 2
	};

	mMeshTriangles =
		new GraphicsEngine::Mesh(verticesTriangles, numberOfVerticesTriangles, indicesTriangles, numberOfIindicesTriangles);

	mMeshSquares =
		new GraphicsEngine::Mesh(verticesSquares, numberOfVerticesSquares, indicesSquares, numberOfIindicesSquares);


	int opaqueTriangles = sMaxNumberOfTriangles / 2;
	mFirstTransparentIndex = opaqueTriangles + sMaxNumberOfSquares;
	mFirstInactiveIndex = sMaxNumberOfTriangles + sMaxNumberOfSquares;

	for (int i = 0; i < sMaxNumberOfTriangles + sMaxNumberOfSquares; ++i)
	{

		bool triangle;
		
		//Create Objects
		mObjectsTransform[i].initializeOnDevice(mDevice);
		
		if (i < opaqueTriangles)
		{
			triangle = true;
			mObjects[i] = GraphicsEngine::Object(mMeshTriangles, mOpaqueMaterialTriangles);
			mObjectsTransform[i].translate((i - (static_cast<int>(sMaxNumberOfTriangles) / 2)) * 1.5f, 1.0f, 0.0f, mDeviceContext);
		}
		else if (i < mFirstTransparentIndex)
		{
			triangle = false;
			mObjects[i] = GraphicsEngine::Object(mMeshSquares, mOpaqueMaterialSquares);
			mObjectsTransform[i].translate(((i - opaqueTriangles) - (static_cast<int>(sMaxNumberOfSquares) / 2)) * 1.5f, 1.0f, 5.0f, mDeviceContext);
		}
		else if (i < mFirstInactiveIndex)
		{
			triangle = true;
			mObjects[i] = GraphicsEngine::Object(mMeshTriangles, mTransparentMaterialTriangles);
			mObjectsTransform[i].translate(((i - sMaxNumberOfSquares) - (static_cast<int>(sMaxNumberOfTriangles) / 2)) * 1.5f, 1.0f, 0.0f, mDeviceContext);
		}

		mObjects[i].initializeOnDevice(mDevice);


		//Create AABBs
		if (triangle)
		{
			mAABBs[i] = GraphicsEngine::AABB(verticesTriangles, numberOfVerticesTriangles);
		}
		else
		{ 
			mAABBs[i] = GraphicsEngine::AABB(verticesSquares, numberOfVerticesSquares);
		}

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