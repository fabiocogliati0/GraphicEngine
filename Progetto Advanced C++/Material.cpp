#include "Material.h"

#include "VertexShader.h"
#include "PixelShader.h"

#include <d3d11_1.h>
#include <DirectXMath.h>


namespace GraphicEngine
{

	Material::Material(
		const DirectX::XMFLOAT4& iAmbiental,
		const DirectX::XMFLOAT4& iDiffusive,
		const DirectX::XMFLOAT4& iSpecular,
		const DirectX::XMFLOAT4& iEmissive,
		float iShininess,
		VertexShader* iVertexShader,
		PixelShader* iPixelShader) : 
			mMaterialStruct(iAmbiental, iDiffusive, iSpecular, iEmissive, iShininess),
			mVertexShader(iVertexShader),
			mPixelShader(iPixelShader)
	{
	}

	Material::~Material()
	{
		release();
	}

	void Material::initializeOnDevice(ID3D11Device* iDevice)
	{
		if (!mMaterialBuffer && iDevice)
		{
			D3D11_BUFFER_DESC bufferDesc;
			bufferDesc.Usage = D3D11_USAGE_DEFAULT;
			bufferDesc.ByteWidth = sizeof(MaterialStruct);
			bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			bufferDesc.CPUAccessFlags = 0;
			bufferDesc.MiscFlags = 0;
			D3D11_SUBRESOURCE_DATA initData;
			initData.pSysMem = &mMaterialStruct;
			HRESULT result = iDevice->CreateBuffer(&bufferDesc, &initData, &mMaterialBuffer);
			assert(SUCCEEDED(result));

			mVertexShader->initOnDevice(iDevice);		//TODO: sono giuste queste due linee?
			mPixelShader->initOnDevice(iDevice);
		}
	}

	void Material::renderSetup(ID3D11DeviceContext* iContext) const
	{
		if (mVertexShader && mPixelShader && mMaterialBuffer && iContext)
		{
			mVertexShader->renderSetup(iContext);
			mPixelShader->renderSetup(iContext);

			iContext->VSSetConstantBuffers(1, 1, &mMaterialBuffer);
		}
	}

	void Material::release()
	{
		if (mMaterialBuffer)
		{
			mMaterialBuffer->Release();
		}
	}

	void Material::setVertexShader(VertexShader* iVertexShader)
	{
		mVertexShader = iVertexShader;
	}

	void Material::setPixelShader(PixelShader* iPixelShader)
	{
		mPixelShader = iPixelShader;
	}

	void Material::setAmbiental(const DirectX::XMFLOAT4& iAmbiental)
	{
		mMaterialStruct.ambiental = iAmbiental;
	}

	void Material::setDiffusive(const DirectX::XMFLOAT4& iDiffusive)
	{
		mMaterialStruct.diffusive = iDiffusive;
	}

	void Material::setSpecular(const DirectX::XMFLOAT4& iSpecular)
	{
		mMaterialStruct.specular = iSpecular;
	}

	void Material::setEmissive(const DirectX::XMFLOAT4& iEmissive)
	{
		mMaterialStruct.emissive = iEmissive;
	}

	void Material::setShininess(float iShininess)
	{
		mMaterialStruct.shininess = iShininess;
	}

	DirectX::XMFLOAT4 Material::getAmbiental() const
	{
		return mMaterialStruct.ambiental;
	}

	DirectX::XMFLOAT4 Material::getDiffusive() const
	{
		return mMaterialStruct.diffusive;
	}

	DirectX::XMFLOAT4 Material::getSpecular() const
	{
		return mMaterialStruct.specular;
	}

	DirectX::XMFLOAT4 Material::getEmissive() const
	{
		return mMaterialStruct.emissive;
	}

	float Material::getShininess() const
	{
		return mMaterialStruct.shininess;
	}


} 