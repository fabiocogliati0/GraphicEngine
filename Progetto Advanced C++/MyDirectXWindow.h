#pragma once

#include "DirectXWindow.h"
#include "Camera.h"
#include "WorldTransform.h"

#include <d3d11.h>

//Forward declarations
namespace GraphicsEngine
{
	class Object;
	class AABB;
}

class MyDirectXWindow : public GraphicsEngine::DirectXWindow
{

private:

	static const long sWindowSizeX = 600L;

	static const long sWindowSizeY = 600L;

	static const int sMultiSampleCount = 1;

	static const unsigned int sMaxNumberOfTriangles = 12000;

	static const unsigned int sMaxNumberOfSquares = 12000;


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

	void createObjects();

	void createDepthStencilState();

	void createRasterizerStates();
	
	void createBlendingStates();


	int mFirstTransparentIndex;

	int mFirstInactiveIndex;

	GraphicsEngine::Camera mCamera;

	GraphicsEngine::WorldTransform* mObjectsTransform;

	GraphicsEngine::Object* mObjects;

	GraphicsEngine::AABB* mAABBs;

	ID3D11DepthStencilState* mDepthStateOff;

	ID3D11DepthStencilState* mDepthStateOn;

	ID3D11RasterizerState* mRasterizerStateBackFaceCulling;

	ID3D11BlendState* mBlendingStateOff;

	ID3D11BlendState* mBlendingStateOn;
};