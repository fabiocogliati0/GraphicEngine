#include "Camera.h"

#include <d3d11.h>
#include <DirectXMath.h>
#include <cassert>

namespace GraphicsEngine
{

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
			bufferDesc.MiscFlags = 0;
			D3D11_SUBRESOURCE_DATA initData;
			initData.pSysMem = &mCameraStruct;
			HRESULT result = iDevice->CreateBuffer(&bufferDesc, &initData, &mCameraBuffer);
			assert(SUCCEEDED(result));
		}
	}

	void Camera::renderSetup(ID3D11DeviceContext* iContext) const
	{
		if (mCameraBuffer && iContext)
		{
			//iContext->UpdateSubresource(mCameraBuffer, 0, nullptr, &mCameraStruct, 0, 0); //questo va fatto quando si modificano i dati
			iContext->VSSetConstantBuffers(1, 1, &mCameraBuffer);
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