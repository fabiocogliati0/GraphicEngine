#pragma once

#include <d3d11_1.h>
#include <DirectXMath.h>
#include <cassert>

namespace GraphicsEngine
{

	const unsigned int MaxLights = 5;

#pragma pack(push, 16)

	struct LightStruct
	{
		DirectX::XMFLOAT4 position[MaxLights];
		DirectX::XMFLOAT4 color[MaxLights];
		DirectX::XMFLOAT4 ambient[MaxLights];
		DirectX::XMFLOAT4 attenuation[MaxLights];
		int usedLights; // quante luci sono utilizzate.
		float padding[3];
	};

#pragma pack(pop)

	//TODO: fare bene
	class Lights
	{

	public:

		Lights()
		{
			mLightsStruct.usedLights = 2;
			// Prima luce
			mLightsStruct.position[0] = DirectX::XMFLOAT4(2.0f, 0.0f, 0.0f, 1.0f);
			mLightsStruct.color[0] = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			mLightsStruct.ambient[0] = DirectX::XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
			mLightsStruct.attenuation[0] = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f);

			// Seconda luce
			mLightsStruct.position[1] = DirectX::XMFLOAT4(2.0f, -2.0f, 0.0f, 1.0f);
			mLightsStruct.color[1] = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
			mLightsStruct.ambient[1] = DirectX::XMFLOAT4(0.3f, 0.1f, 0.1f, 1.0f);
			mLightsStruct.attenuation[1] = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f);
		}

		void initializeOnDevice(ID3D11Device* iDevice)
		{
			D3D11_BUFFER_DESC bufferDesc;
			bufferDesc.Usage = D3D11_USAGE_DEFAULT;
			bufferDesc.ByteWidth = sizeof(LightStruct);
			bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			bufferDesc.CPUAccessFlags = 0;
			bufferDesc.MiscFlags = 0;
			D3D11_SUBRESOURCE_DATA initData;
			initData.pSysMem = &mLightsStruct;
			HRESULT result = iDevice->CreateBuffer(&bufferDesc, &initData, &mLightsBuffer);
			assert(SUCCEEDED(result));
		}

		void renderSetup(ID3D11DeviceContext* iContext)
		{
			iContext->UpdateSubresource(mLightsBuffer, 0, nullptr, &mLightsStruct, 0, 0);
		}

	private:

		LightStruct mLightsStruct;
		ID3D11Buffer* mLightsBuffer;

	};


}