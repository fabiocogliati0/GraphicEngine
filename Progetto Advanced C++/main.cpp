#include "Window.h"
#include "ShaderCompiler.h"

#include <Windows.h>
#include <d3d11.h>

const int gMultiSampleCount = 1;


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	GraphicEngine::Window window(hInstance, nCmdShow, gMultiSampleCount, L"Advanced C++ Project", L"Project", 600L, 600L);

	//TODO: mettere path relativi
	ID3D11VertexShader* vertexShader = GraphicEngine::compileVertexShaders(L"C:/Users/Fabio/Documents/Visual Studio 2013/Projects/Progetto Advanced C++/Debug/vertexShader.cso", window.getRender()->getDevice());
	ID3D11PixelShader* pixelShader = GraphicEngine::compilePixelShaders(L"C:/Users/Fabio/Documents/Visual Studio 2013/Projects/Progetto Advanced C++/Debug/pixelShader.cso", window.getRender()->getDevice());;

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

	return static_cast<int>(msg.wParam);

}