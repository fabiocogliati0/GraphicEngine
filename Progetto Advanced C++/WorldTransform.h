#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

namespace GraphicsEngine
{

	class WorldTransform
	{

	public:

		WorldTransform();

		WorldTransform(const DirectX::XMMATRIX& iWorld);

		~WorldTransform();

		void initializeOnDevice(ID3D11Device* iDevice);

		void renderSetup(ID3D11DeviceContext* iContext) const;

		void release();

		void translate(float iX, float iY, float iZ);	//TODO: questo dovrebbe essere tolto

		void translate(float iX, float iY, float iZ, ID3D11DeviceContext* iContext);

		void scale(float iX, float iY, float iZ);

		void rotateX(float iAngle);

		void rotateY(float iAngle);

		void rotateZ(float iAngle);

	private:

#pragma pack(push, 16)

		struct WorldTransformStruct
		{
			DirectX::XMMATRIX world;

			WorldTransformStruct(const DirectX::XMMATRIX& iWorld):
				world(iWorld)
			{
			}
		};

#pragma pack(pop)

		WorldTransformStruct mWorldTransformStruct;
		ID3D11Buffer* mWorldTransformBuffer;

	};


}