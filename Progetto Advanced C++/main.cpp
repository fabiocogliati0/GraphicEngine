#include "GraphicsEngine.h"

#include <Windows.h>
#include <d3d11.h>
#include <DirectXMath.h>

const long gWindowSizeX = 600L;
const long gWindowSizeY = 600L;
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
	//Calculate aspetct ratio
	const float gAspectRatio = static_cast<float>(gWindowSizeX) / gWindowSizeY;

	//Create Window
	GraphicsEngine::DirectXWindow window(hInstance, nCmdShow, gMultiSampleCount, L"Advanced C++ Project", L"Project", gWindowSizeX, gWindowSizeY);

	//Create Camera
	GraphicsEngine::Camera camera(0.0f, 0.0f, -2.0f, 0.0f, 0.0f, 1.0f, gAspectRatio);
	window.setCamera(camera);

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

	//Create Trasnform
	GraphicsEngine::WorldTransform transform;

	//Create Object
	GraphicsEngine::Object* object = new GraphicsEngine::Object(mesh, material, transform);
	window.addObjectToRender(object);

	//Run program
	int executionReturn = window.run();

	//clean								//TODO: non ce n'è bisogno perchè non ho fatto grab, pensare a cosa sarebbe la cosa migliore da fare
	/*material->release();
	material = nullptr;
	vertexShader->release();
	vertexShader = nullptr;
	pixelShader->release();
	pixelShader = nullptr;
	mesh->release();
	mesh = nullptr;*/


	return executionReturn;

}