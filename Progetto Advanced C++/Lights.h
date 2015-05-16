//#pragma once
//
//#include <d3d11_1.h>
//#include <DirectXMath.h>
//#include <cassert>
//
//namespace GraphicEngine
//{
//
//	const unsigned int MaxLights = 5;
//
//#pragma pack(push, 16)
//
//	struct LightStruct
//	{
//		DirectX::XMFLOAT4 position[MaxLights];
//		DirectX::XMFLOAT4 color[MaxLights];
//		DirectX::XMFLOAT4 ambient[MaxLights];
//		DirectX::XMFLOAT4 attenuation[MaxLights];
//		int usedLights; // quante luci sono utilizzate.
//		float padding[3];
//	};
//
//#pragma pack(pop)
//
//
//}