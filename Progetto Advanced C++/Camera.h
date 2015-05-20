#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

namespace GraphicEngine
{

	class Camera
	{

	public:

	private:

#pragma pack(push, 16)

		struct CameraStruct
		{
			DirectX::XMMATRIX view;
			DirectX::XMMATRIX projection;
			DirectX::XMFLOAT4 cameraPosition;
		};

#pragma pack(pop)

	};

}