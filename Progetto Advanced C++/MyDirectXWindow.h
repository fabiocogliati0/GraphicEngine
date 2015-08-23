#pragma once

#include "DirectXWindow.h"
#include "Camera.h"
#include "Object.h"

#include <d3d11.h>

class MyDirectXWindow : public GraphicsEngine::DirectXWindow
{

private:

	static const long sWindowSizeX = 600L;

	static const long sWindowSizeY = 600L;

	static const int sMultiSampleCount = 1;

	static const unsigned int sMaxNumberOfTriangles = 10;

	static const unsigned int sMaxNumberOfSquares = 8;


	static const unsigned int sLayoutVertexSize = 2;

	static const D3D11_INPUT_ELEMENT_DESC* sLayoutVertex;

	static const LPCWSTR sWindowTitle;

	static const LPCWSTR sWindowClassName;

public:

	MyDirectXWindow(const HINSTANCE iHInstance, int iNCmdShow);

	~MyDirectXWindow();

	int run();

private :

	void init();

	void moveObjects();

	void renderObjects();

	void createTrinangles();

	void createSquares();

	void createDepthStencilState();

	void createRasterizerStates();
	
	void createBlendingStates();


	GraphicsEngine::Camera mCamera;

	GraphicsEngine::Object mTriangles[sMaxNumberOfTriangles];

	GraphicsEngine::Object mSquares[sMaxNumberOfSquares];

	ID3D11DepthStencilState* mDepthStateOff;

	ID3D11DepthStencilState* mDepthStateOn;

	ID3D11RasterizerState* mRasterizerStateBackFaceCulling;

	ID3D11BlendState* mBlendingStateOff;

	ID3D11BlendState* mBlendingStateOn;
};