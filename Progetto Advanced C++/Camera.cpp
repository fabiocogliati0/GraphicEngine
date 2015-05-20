#include "Camera.h"

#include <d3d11.h>
#include <DirectXMath.h>
#include <cassert>

namespace GraphicEngine
{

	Camera::Camera(
		const DirectX::XMVECTOR& iEyePosition,
		const DirectX::XMVECTOR& iEyeDirection,
		const DirectX::XMVECTOR& iUpDirection,
		float iFovAngleY,
		float iAspectRatio,
		float iNearZ,
		float iFarZ,
		const DirectX::XMFLOAT4& iCameraPosition,
		ID3D11Device* iDevice) :
			mCameraStruct(
				DirectX::XMMatrixLookToLH(iEyePosition, iEyeDirection, iUpDirection),
				DirectX::XMMatrixPerspectiveFovLH(iFovAngleY, iAspectRatio, iNearZ, iFarZ),
				iCameraPosition)
	{
		HRESULT result;

		//Create Camera buffer
		D3D11_BUFFER_DESC bufferDesc;
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.ByteWidth = sizeof(CameraStruct);
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDesc.CPUAccessFlags = 0;
		result = iDevice->CreateBuffer(&bufferDesc, nullptr, &mCameraBuffer);
		assert(SUCCEEDED(result));

	}

	Camera::~Camera()
	{
		if (mCameraBuffer)
			mCameraBuffer->Release();
	}

	void Camera::renderSetup(ID3D11DeviceContext* iContext) const
	{
		iContext->UpdateSubresource(mCameraBuffer, 0, nullptr, &mCameraStruct, 0, 0);
	}

}