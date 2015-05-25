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
		const DirectX::XMFLOAT4& iCameraPosition) :
			mCameraStruct(
				DirectX::XMMatrixLookToLH(iEyePosition, iEyeDirection, iUpDirection),
				DirectX::XMMatrixPerspectiveFovLH(iFovAngleY, iAspectRatio, iNearZ, iFarZ),
				iCameraPosition)
	{
	}

	Camera::~Camera()
	{
		release();
	}

	void Camera::initializeOnDevice(ID3D11Device* iDevice)
	{
		if (!mCameraBuffer && iDevice)
		{
			//Create Camera buffer
			D3D11_BUFFER_DESC bufferDesc;
			bufferDesc.Usage = D3D11_USAGE_DEFAULT;
			bufferDesc.ByteWidth = sizeof(CameraStruct);
			bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			bufferDesc.CPUAccessFlags = 0;
			HRESULT result = iDevice->CreateBuffer(&bufferDesc, nullptr, &mCameraBuffer);
			assert(SUCCEEDED(result));
		}
	}

	void Camera::renderSetup(ID3D11DeviceContext* iContext) const
	{
		if (mCameraBuffer && iContext)
		{
			iContext->UpdateSubresource(mCameraBuffer, 0, nullptr, &mCameraStruct, 0, 0);
		}
	}

	void Camera::release()
	{
		if (mCameraBuffer)
		{
			mCameraBuffer->Release();
		}
	}

}