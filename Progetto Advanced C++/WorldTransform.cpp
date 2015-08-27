#include "WorldTransform.h"

#include <d3d11.h>
#include <DirectXMath.h>
#include <cassert>

namespace GraphicsEngine
{

	WorldTransform::WorldTransform() :
		WorldTransform(DirectX::XMMatrixIdentity())
	{
	}

	WorldTransform::WorldTransform(const DirectX::XMMATRIX& iWorld) :
		mWorldTransformStruct(iWorld),
		mWorldTransformBuffer(nullptr)
	{
	}

	WorldTransform::WorldTransform(const WorldTransform& iOther) :
		mWorldTransformStruct(iOther.mWorldTransformStruct),
		mWorldTransformBuffer(nullptr)
	{
	}

	WorldTransform::~WorldTransform()
	{
		if (mWorldTransformBuffer)
		{
			mWorldTransformBuffer->Release();
		}
	}

	WorldTransform& WorldTransform:: operator = (const WorldTransform& iOther)
	{
		if (this != &iOther)
		{
			mWorldTransformStruct = iOther.mWorldTransformStruct;
			mWorldTransformBuffer = nullptr;
		}
		return *this;
	}

	void WorldTransform::initializeOnDevice(ID3D11Device* iDevice)
	{

		assert(iDevice);

		if (!mWorldTransformBuffer)
		{
			//Create transformations buffer
			D3D11_BUFFER_DESC bufferDesc;
			bufferDesc.Usage = D3D11_USAGE_DEFAULT;
			bufferDesc.ByteWidth = sizeof(WorldTransformStruct);
			bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			bufferDesc.CPUAccessFlags = 0;
			bufferDesc.MiscFlags = 0;
			D3D11_SUBRESOURCE_DATA initData;
			initData.pSysMem = &mWorldTransformStruct;
			HRESULT result = iDevice->CreateBuffer(&bufferDesc, &initData, &mWorldTransformBuffer);
			assert(SUCCEEDED(result));
		}
	}

	void WorldTransform::renderSetup(ID3D11DeviceContext* iContext) const
	{
		assert(iContext);

		if (mWorldTransformBuffer)
		{
			iContext->VSSetConstantBuffers(0, 1, &mWorldTransformBuffer);
		}	
	}

	void WorldTransform::translate(float iX, float iY, float iZ, ID3D11DeviceContext* iContext)
	{
		mWorldTransformStruct.world =
			DirectX::XMMatrixMultiply(mWorldTransformStruct.world, DirectX::XMMatrixTranslation(iX, iY, iZ));

		iContext->UpdateSubresource(mWorldTransformBuffer, 0, nullptr, &mWorldTransformStruct, 0, 0);
	}


	DirectX::XMFLOAT3 WorldTransform::getGlobalPosition(const DirectX::XMFLOAT3 &iLocalPosition) const
	{
		DirectX::XMVECTOR pos = DirectX::XMLoadFloat3(&iLocalPosition);
		pos = DirectX::XMVector3Transform(pos, mWorldTransformStruct.world);
		DirectX::XMFLOAT3 result;
		DirectX::XMStoreFloat3(&result, pos);
		return result;
	}
}