#include "Material.h"

#pragma once

#include "VertexShader.h"
#include "PixelShader.h"

#include <d3d11_1.h>
#include <DirectXMath.h>


namespace GraphicEngine
{


	Material::Material(
		const DirectX::XMFLOAT4& iAmbiental,
		const DirectX::XMFLOAT4& iDiffuse,
		const DirectX::XMFLOAT4& iSpecular,
		const DirectX::XMFLOAT4& iEmissive,
		float iShininess,
		const VertexShader* iVertexShader,
		const PixelShader* iPixelShader,
		ID3D11Device* iDevice
		) : mMaterialStruct(iAmbiental, iDiffuse, iSpecular, iEmissive, iShininess)
	{

		//sta roba andrebbe fatta nel precostruttore
		mMaterialStruct.ambiental = iAmbiental;
		mMaterialStruct.diffusive = iDiffuse;
		mMaterialStruct.specular = iSpecular;
		mMaterialStruct.emissive = iEmissive;
		mMaterialStruct.shininess = iShininess;

		size_t debug = sizeof(mMaterialStruct);

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

		mVertexShader = iVertexShader;
		mPixelShader = iPixelShader;
	}

	void Material::renderSetup(ID3D11DeviceContext* iContext) const
	{
		mVertexShader->renderSetup(iContext);
		mPixelShader->renderSetup(iContext);

		iContext->VSSetConstantBuffers(1, 1, &mMaterialBuffer);
	}

	void Material::setVertexShader(const VertexShader* iVertexShader)
	{
		mVertexShader = iVertexShader;
	}

	void Material::setPixelShader(const PixelShader* iPixelShader)
	{
		mPixelShader = iPixelShader;
	}

}