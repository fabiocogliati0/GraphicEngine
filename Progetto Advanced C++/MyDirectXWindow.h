#pragma once

#include "DirectXWindow.h"

#include "GraphicsEngine.h"

const LPCWSTR gWindowTitle = L"Advanced C++ Project";
const LPCWSTR gWindowClassName = L"Project";

class MyDirectXWindow : public GraphicsEngine::DirectXWindow
{

private:

	static const long gWindowSizeX = 600L;
	static const long gWindowSizeY = 600L;
	static const int gMultiSampleCount = 1;
	static const unsigned int layoutVertexSize = 2;

	static const unsigned int gMaxNumberOfTriangles = 10;
	static const unsigned int gMaxNumberOfSquares = 8;

public:

	MyDirectXWindow(const HINSTANCE iHInstance, int iNCmdShow);

	int run();

private :

	void init();

	void createTrinangles();

	void createSquares();

	void createBlendingStates();

	GraphicsEngine::Object mTriangles[gMaxNumberOfTriangles];
	GraphicsEngine::Object mSquares[gMaxNumberOfSquares];

	ID3D11BlendState* mBlendingStateOff;
	ID3D11BlendState* mBlendingStateOn;

	GraphicsEngine::Camera* mCamera;

	const float mAspectRatio;

};