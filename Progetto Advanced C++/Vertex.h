#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

namespace GraphicsEngine
{

	struct Vertex
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT3 normal;
	};

}