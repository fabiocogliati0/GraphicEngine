#include "WorldTransform.h"

#include <d3d11.h>
#include <DirectXMath.h>
#include <cassert>

namespace GraphicsEngine
{

	WorldTransform::WorldTransform()
		:WorldTransform(DirectX::XMMatrixIdentity())
	{
	}

	WorldTransform::WorldTransform(const DirectX::XMMATRIX& iWorld) :
			mWorldTransformStruct(iWorld)
	{
	}

	WorldTransform::~WorldTransform()
	{
		release();
	}

	void WorldTransform::initializeOnDevice(ID3D11Device* iDevice)
	{
		if (!mWorldTransformBuffer && iDevice)
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
		if (mWorldTransformBuffer && iContext)
		{
			//iContext->UpdateSubresource(mWorldTransformBuffer, 0, nullptr, &mWorldTransformStruct, 0, 0);	//questo va fatto quando si modifica la trasform
			iContext->VSSetConstantBuffers(0, 1, &mWorldTransformBuffer);
		}	
	}

	void WorldTransform::release()
	{
		if (mWorldTransformBuffer)
		{
			mWorldTransformBuffer->Release();
		}
	}

	void WorldTransform::translate(float iX, float iY, float iZ)
	{
		mWorldTransformStruct.world =
			DirectX::XMMatrixMultiply(mWorldTransformStruct.world, DirectX::XMMatrixTranslation(iX, iY, iZ));
	}

	void WorldTransform::scale(float iX, float iY, float iZ)
	{
		mWorldTransformStruct.world =
			DirectX::XMMatrixMultiply(mWorldTransformStruct.world, DirectX::XMMatrixScaling(iX, iY, iZ));
	}

	void WorldTransform::rotateX(float iAngle)
	{
		mWorldTransformStruct.world =
			DirectX::XMMatrixMultiply(mWorldTransformStruct.world, DirectX::XMMatrixRotationX(iAngle));
	}

	void WorldTransform::rotateY(float iAngle)
	{
		mWorldTransformStruct.world =
			DirectX::XMMatrixMultiply(mWorldTransformStruct.world, DirectX::XMMatrixRotationY(iAngle));
	}

	void WorldTransform::rotateZ(float iAngle)
	{
		mWorldTransformStruct.world =
			DirectX::XMMatrixMultiply(mWorldTransformStruct.world, DirectX::XMMatrixRotationZ(iAngle));
	}

}