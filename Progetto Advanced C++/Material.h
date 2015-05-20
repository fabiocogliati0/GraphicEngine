#pragma once

#include "VertexShader.h"
#include "PixelShader.h"

#include <d3d11_1.h>
#include <DirectXMath.h>


namespace GraphicEngine
{
	
	class Material
	{

	public:

		Material(
			const DirectX::XMFLOAT4& iAmbiental,
			const DirectX::XMFLOAT4& iDiffusive,
			const DirectX::XMFLOAT4& iSpecular,
			const DirectX::XMFLOAT4& iEmissive,
			float iShininess,
			const VertexShader* iVertexShader,
			const PixelShader* iPixelShader,
			ID3D11Device* iDevice);

		~Material();

		void renderSetup(ID3D11DeviceContext* iContext) const;

		void setVertexShader(const VertexShader* iVertexShader);

		void setPixelShader(const PixelShader* iPixelShader);

		void setAmbiental(const DirectX::XMFLOAT4& iAmbiental);

		void setDiffusive(const DirectX::XMFLOAT4& iDiffusive);

		void setSpecular(const DirectX::XMFLOAT4& iSpecular);

		void setEmissive(const DirectX::XMFLOAT4& iEmissive);

		void setShininess(float iShininess);

		DirectX::XMFLOAT4 getAmbiental() const;

		DirectX::XMFLOAT4 getDiffusive() const;

		DirectX::XMFLOAT4 getSpecular() const;

		DirectX::XMFLOAT4 getEmissive() const;

		float getShininess() const;

	private:

#pragma pack(push, 16)

		struct MaterialStruct
		{
			DirectX::XMFLOAT4 ambiental;
			DirectX::XMFLOAT4 diffusive;
			DirectX::XMFLOAT4 specular;
			DirectX::XMFLOAT4 emissive;
			float shininess;
			float padding[3];

			MaterialStruct(
				const DirectX::XMFLOAT4& iAmbiental,
				const DirectX::XMFLOAT4& iDiffusive,
				const DirectX::XMFLOAT4& iSpecular,
				const DirectX::XMFLOAT4& iEmissive,
				float iShininess) :
					ambiental(iAmbiental),
					diffusive(iDiffusive),
					specular(iSpecular),
					emissive(iEmissive), 
					shininess(iShininess)
			{
			}

		};

#pragma pack(pop)

		MaterialStruct mMaterialStruct;
		const VertexShader* mVertexShader;
		const PixelShader* mPixelShader;
		ID3D11Buffer* mMaterialBuffer;

	};

}