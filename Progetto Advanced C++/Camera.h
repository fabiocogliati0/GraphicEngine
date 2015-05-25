#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

namespace GraphicEngine
{

	//TODO: mettere giù un po' meglio il design per permettere di definire diverse projection
	//		esporre dei XMVECTOR non è una grandissima idea

	class Camera
	{

	public:

		Camera(
			const DirectX::XMVECTOR& iEyePosition,
			const DirectX::XMVECTOR& iEyeDirection,
			const DirectX::XMVECTOR& iUpDirection,
			float iFovAngleY,
			float iAspectRatio,
			float iNearZ,
			float iFarZ,
			const DirectX::XMFLOAT4& iCameraPosition);

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