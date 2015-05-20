#include "WorldTransform.h"

#include <d3d11.h>
#include <DirectXMath.h>
#include <cassert>

namespace GraphicEngine
{

	WorldTransform::WorldTransform(
		const DirectX::XMMATRIX& iWorld,
		ID3D11Device* iDevice) :
		mWorldTransformStruct(iWorld)
	{

		HRESULT result;

		//Create transformations buffer
		D3D11_BUFFER_DESC bufferDesc;
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.ByteWidth = sizeof(WorldTransformStruct);
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDesc.CPUAccessFlags = 0;
		result = iDevice->CreateBuffer(&bufferDesc, nullptr, &mWorldTransformBuffer);
		assert(SUCCEEDED(result));
	}

	WorldTransform::~WorldTransform()
	{
		mWorldTransformBuffer->Release();
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