#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

namespace GraphicEngine
{

	struct Vertex
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT3 normal;
	};

}