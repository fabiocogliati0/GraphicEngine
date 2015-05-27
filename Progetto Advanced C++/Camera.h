#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

namespace GraphicsEngine
{

	//TODO: decidere dove mettere le globali qui sotto

	//view
	const float gUpDirectionX = 0.0f;
	const float gUpDirectionY = 1.0f;
	const float gUpDirectionZ = 0.0f;

	//Perspective projection consts
	const float gFovAngleY = 45.0f;
	const float gNearZ = 0.1f;
	const float gFarZ = 100.0f;

	//TODO: descrizione, penso di farla che può essere ruotata solo sull'asse y e spostata dove si vuole
	class Camera
	{

	public:

		Camera();

		Camera(
			float iCameraPosX,
			float iCameraPosY,
			float iCameraPosZ,
			float iFocusPointX,
			float iFocusPointY,
			float iFocusPointZ,
			float iAspectRatio);

		~Camera();

		void initializeOnDevice(ID3D11Device* iDevice);

		void renderSetup(ID3D11DeviceContext* iContext) const;

	private:

#pragma pack(push, 16)

		struct CameraStruct
		{
			DirectX::XMMATRIX view;
			DirectX::XMMATRIX projection;
			DirectX::XMFLOAT4 cameraPosition;

			CameraStruct(){}	//TODO: togliere se riesco a iniziallizare la camera nell initializer list, ma mi sembra dura

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