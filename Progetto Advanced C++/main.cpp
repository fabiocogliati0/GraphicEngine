#include "Window.h"
#include "Vertex.h"
#include "Mesh.h"
#include "Material.h"
#include "WorldTransform.h"
#include "PixelShader.h"
#include "VertexShader.h"
#include "Object.h"

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
	GraphicEngine::Window window(hInstance, nCmdShow, gMultiSampleCount, L"Advanced C++ Project", L"Project", 600L, 600L);
	ID3D11DeviceContext* deviceContext = window.getRender()->getDeviceContext();

	//Create Vertex Shader
	GraphicEngine::VertexShader* vertexShader = 
		new GraphicEngine::VertexShader(
		L"C:/Users/Fabio/Documents/Visual Studio 2013/Projects/Progetto Advanced C++/Debug/vertexShader.cso",
		layoutVertex,
		layoutVertexSize);
	
	//Create Pixel Shader
	GraphicEngine::PixelShader* pixelShader = 
		new GraphicEngine::PixelShader(L"C:/Users/Fabio/Documents/Visual Studio 2013/Projects/Progetto Advanced C++/Debug/pixelShader.cso");

	//Create Material
	DirectX::XMFLOAT4 ambiental(0.1f, 0.3f, 0.3f, 1.0f);
	DirectX::XMFLOAT4 diffusive(0.2f, 0.5f, 0.5f, 1.0f);
	DirectX::XMFLOAT4 specular(1.0f, 1.0f, 1.0f, 1.0f);
	DirectX::XMFLOAT4 emissive (0.0f, 0.0f, 0.0f, 1.0f);
	float shininess = 128;

	GraphicEngine::Material* material =
		new GraphicEngine::Material(ambiental, diffusive, specular, emissive, shininess, vertexShader, pixelShader);

	//Create Mesh
	GraphicEngine::Vertex vertices[] =
	{
		{ DirectX::XMFLOAT3(-1.0f, 1.0f, -1.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f) },
		{ DirectX::XMFLOAT3(1.0f, 1.0f, -1.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f) },
		{ DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f) }
	};

	unsigned int indices[] =
	{
		0, 1, 2
	};

	GraphicEngine::Mesh* mesh =
		new GraphicEngine::Mesh(vertices, 3, indices, 3);

	
	//Create Trasnform
	GraphicEngine::WorldTransform* transform = new GraphicEngine::WorldTransform();

	//Create Object
	GraphicEngine::Object object(mesh, material, transform);

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
			object.render(deviceContext);
		}
	}


	//clean
	delete material;
	delete vertexShader;
	delete pixelShader;
	delete mesh;
	delete transform;

	material = nullptr;
	vertexShader = nullptr;
	pixelShader = nullptr;
	mesh = nullptr;
	transform = nullptr;

	return static_cast<int>(msg.wParam);

}