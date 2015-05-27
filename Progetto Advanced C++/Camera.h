#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

namespace GraphicsEngine
{

	//TODO: mettere giù un po' meglio il design per permettere di definire diverse projection
	//		esporre dei XMVECTOR non è una grandissima idea


	//view
	const float gFocusPointX = 0.0f;
	const float gFocusPointY = 0.0f;
	const float gFocusPointZ = 1.0f;
	const float gUpDirectionX = 0.0f;
	const float gUpDirectionY = 1.0f;
	const float gUpDirectionZ = 0.0f;

	//Perspective projection consts
	const float gFovAngleY = 45.0f;
	const float gNearZ = 0.1f;
	const float gFarZ = 100.0f;

	class Camera
	{

	public:

		Camera()
			: Camera(0.0f, 0.0f, 0.0f, 1.0f)
		{

		}

		Camera(float cameraPosX, float cameraPosY, float cameraPosZ, float iAspectRatio)
		{

			//view
			DirectX::XMVECTOR pos(DirectX::XMVectorSet(cameraPosX, cameraPosY, cameraPosZ, 1.0f));
			DirectX::XMVECTOR focus(DirectX::XMVectorSet(gFocusPointX, gFocusPointY, gFocusPointZ, 1.0f));
			DirectX::XMVECTOR up(DirectX::XMVectorSet(gUpDirectionX, gUpDirectionY, gUpDirectionZ, 0.0f));
			mCameraStruct.view = DirectX::XMMatrixLookAtLH(pos, focus, up);

			//camera pos
			mCameraStruct.cameraPosition = DirectX::XMFLOAT4(cameraPosX, cameraPosY, cameraPosZ, 1.0f);	// TODO: non sono sicuro su questo 1.0f

			//projection
			mCameraStruct.projection = DirectX::XMMatrixPerspectiveFovLH(
				DirectX::XMConvertToRadians(gFovAngleY),
				iAspectRatio,
				gNearZ,
				gFarZ);
		}

		~Camera();

		void initializeOnDevice(ID3D11Device* iDevice);

		void renderSetup(ID3D11DeviceContext* iContext) const;

		void release();

	private:

#pragma pack(push, 16)

		struct CameraStruct
		{
			DirectX::XMMATRIX view;
			DirectX::XMMATRIX projection;
			DirectX::XMFLOAT4 cameraPosition;

			CameraStruct(){} //TOGLIERE ASSOULUTAMENTE

			CameraStruct(
				const DirectX::XMMATRIX& iWiew,
				const DirectX::XMMATRIX& iProjection,
				const DirectX::XMFLOAT4& iCameraPosition) :
					view(iWiew),
					projection(iProjection),
					cameraPosition(iCameraPosition)
			{
			}

		};

#pragma pack(pop)

		CameraStruct mCameraStruct;
		ID3D11Buffer* mCameraBuffer;

	};

}