#include "GraphicsEngine.h"

#include <Windows.h>
#include <d3d11.h>
#include <DirectXMath.h>

const int gMultiSampleCount = 1;

//TODO : SPOSTARE DA QUI
const unsigned int layoutVertexSize = 2;
D3D11_INPUT_ELEMENT_DESC layoutVertex[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	//Create Window
	GraphicsEngine::Window window(hInstance, nCmdShow, gMultiSampleCount, L"Advanced C++ Project", L"Project", 600L, 600L);
	//ID3D11DeviceContext* deviceContext = window.getRender()->getDeviceContext();

	//Create Vertex Shader
	GraphicsEngine::VertexShader* vertexShader =
		new GraphicsEngine::VertexShader(
		L"C:/Users/Fabio/Documents/Visual Studio 2013/Projects/Progetto Advanced C++/Debug/PhongShadingVS.cso",
		layoutVertex,
		layoutVertexSize);
	
	//Create Pixel Shader
	GraphicsEngine::PixelShader* pixelShader = 
		new GraphicsEngine::PixelShader(L"C:/Users/Fabio/Documents/Visual Studio 2013/Projects/Progetto Advanced C++/Debug/PhongShadingPS.cso");

	//Create Material
	DirectX::XMFLOAT4 ambiental(0.1f, 0.3f, 0.3f, 1.0f);
	DirectX::XMFLOAT4 diffusive(0.2f, 0.5f, 0.5f, 1.0f);
	DirectX::XMFLOAT4 specular(1.0f, 1.0f, 1.0f, 1.0f);
	DirectX::XMFLOAT4 emissive (0.0f, 0.0f, 0.0f, 1.0f);
	float shininess = 128;

	GraphicsEngine::Material* material =
		new GraphicsEngine::Material(ambiental, diffusive, specular, emissive, shininess, vertexShader, pixelShader);

	//Create Mesh
	GraphicsEngine::Vertex vertices[] =
	{
		{ DirectX::XMFLOAT3(-1.0f, 1.0f, -1.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f) },
		{ DirectX::XMFLOAT3(1.0f, 1.0f, -1.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f) },
		{ DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f), DirectX::XMFLOAT3(1.0f, 1.0f, 0.0f) },
		{ DirectX::XMFLOAT3(-1.0f, 1.0f, 1.0f), DirectX::XMFLOAT3(1.0f, 1.0f, 0.0f) },
		{ DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT3(1.0f, -1.0f, -1.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT3(1.0f, -1.0f, 1.0f), DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-1.0f, -1.0f, 1.0f), DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f) },
	};

	unsigned int indices[] =
	{
		3, 1, 0,
		2, 1, 3, // Prima faccia cubo

		0, 5, 4,
		1, 5, 0, // Seconda faccia cubo

		3, 4, 7,
		0, 4, 3, // Terza faccia cubo

		1, 6, 5,
		2, 6, 1, // Quarta faccia cubo

		2, 7, 6,
		3, 7, 2, // Quinta faccia cubo

		6, 4, 5,
		7, 4, 6, // Sesta faccia cubo
	};

	GraphicsEngine::Mesh* mesh =
		new GraphicsEngine::Mesh(vertices, 8, indices, 36);

	
	//Create Trasnform
	GraphicsEngine::WorldTransform transform;

	//Create Object
	GraphicsEngine::Object* object = new GraphicsEngine::Object(mesh, material, transform);
	window.addObjectToRender(object);


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


	//clean
	material->release();
	material = nullptr;

	vertexShader->release();
	vertexShader = nullptr;
	
	pixelShader->release();
	pixelShader = nullptr;
	
	mesh->release();
	mesh = nullptr;

	return static_cast<int>(msg.wParam);

}