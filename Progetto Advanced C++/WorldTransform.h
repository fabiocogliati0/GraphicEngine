#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

namespace GraphicEngine
{

	class WorldTransform
	{

	public:

		WorldTransform(
			const DirectX::XMMATRIX& iWorld,
			ID3D11Device* iDevice);

		~WorldTransform();

		void translate(float iX, float iY, float iZ);

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