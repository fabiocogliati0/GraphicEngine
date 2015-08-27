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

		WorldTransform(const WorldTransform& iOther);

		~WorldTransform();

		WorldTransform& operator= (const WorldTransform& iOther);

		void initializeOnDevice(ID3D11Device* iDevice);

		void renderSetup(ID3D11DeviceContext* iContext) const;

		void translate(float iX, float iY, float iZ, ID3D11DeviceContext* iContext);

		DirectX::XMFLOAT3 getGlobalPosition(const DirectX::XMFLOAT3 &iLocalPosition) const;

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