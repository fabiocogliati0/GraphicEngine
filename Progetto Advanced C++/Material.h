#pragma once

#include <d3d11_1.h>
#include <DirectXMath.h>


namespace GraphicEngine
{

#pragma pack(push, 16)
	
	struct MaterialStruct
	{
		DirectX::XMFLOAT4 ambiental;
		DirectX::XMFLOAT4 diffusive;
		DirectX::XMFLOAT4 specular;
		DirectX::XMFLOAT4 emissive;
		float shininess;
		float padding[3];
	};

#pragma pack(pop)
	

	class Material
	{

		public:

		Material(
			const DirectX::XMFLOAT4& iAmbiental,
			const DirectX::XMFLOAT4& iDiffuse,
			const DirectX::XMFLOAT4& iSpecular,
			const DirectX::XMFLOAT4& iEmissive,
			float iShininess,
			ID3D11ShaderResourceView* iTextureResourceView,
			ID3D11VertexShader* iVertexShader,
			ID3D11PixelShader* iPixelShader,
			ID3D11Device* iDevice
			)
		{

			//sta roba andrebbe fatta nel precostruttore
			mMaterialStruct.ambiental = iAmbiental;
			mMaterialStruct.diffusive = iDiffuse;
			mMaterialStruct.specular = iSpecular;
			mMaterialStruct.emissive = iEmissive;
			mMaterialStruct.shininess = iShininess;

			D3D11_BUFFER_DESC bufferDesc;
			D3D11_SUBRESOURCE_DATA initData;
			HRESULT result;

			bufferDesc.Usage = D3D11_USAGE_DEFAULT;
			bufferDesc.ByteWidth = sizeof(MaterialStruct);
			bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			bufferDesc.CPUAccessFlags = 0;
			bufferDesc.MiscFlags = 0;
			initData.pSysMem = &mMaterialStruct;
			result = iDevice->CreateBuffer(&bufferDesc, &initData, &mMaterialBuffer);
			assert(SUCCEEDED(result));

			mTextureResourceView = iTextureResourceView;
			mVertexShader = iVertexShader;
			mPixelShader = iPixelShader;
		}

		void renderSetup(ID3D11DeviceContext* iContext)
		{
			//TODO settare anche la texture
			iContext->PSSetShaderResources(0, 1, &mTextureResourceView);
			iContext->VSSetShader(mVertexShader, nullptr, 0);
			iContext->PSSetShader(mPixelShader, nullptr, 0);

			iContext->VSSetConstantBuffers(1, 1, &mMaterialBuffer);
		}

		void setTexture(ID3D11ShaderResourceView* iTextureResourceView)
		{
			mTextureResourceView = iTextureResourceView;
		}

		const MaterialStruct& getMaterialStruct() const
		{
			return mMaterialStruct;
		}


	private:

		MaterialStruct mMaterialStruct;

		ID3D11ShaderResourceView* mTextureResourceView;
		ID3D11VertexShader* mVertexShader;
		ID3D11PixelShader* mPixelShader;
		ID3D11Buffer* mMaterialBuffer;

	};

}