#include "Material.h"

#include "VertexShader.h"
#include "PixelShader.h"

#include <d3d11_1.h>
#include <DirectXMath.h>


namespace GraphicsEngine
{

	Material::Material(
		const DirectX::XMFLOAT4& iColor,
		VertexShader* iVertexShader,
		PixelShader* iPixelShader) :
			mMaterialStruct(iColor),
			mVertexShader(iVertexShader),
			mPixelShader(iPixelShader),
			mMaterialBuffer(nullptr)
	{
		//mVertexShader->grab();
		//mPixelShader->grab();
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

			iContext->VSSetConstantBuffers(2, 1, &mMaterialBuffer);
		}
	}

	void Material::setVertexShader(VertexShader* iVertexShader)
	{
		//mVertexShader->release();
		mVertexShader = iVertexShader;
		//mVertexShader->grab();
	}

	void Material::setPixelShader(PixelShader* iPixelShader)
	{
		//mPixelShader->release();
		mPixelShader = iPixelShader;
		//mPixelShader->grab();
	}

	void Material::setColor(const DirectX::XMFLOAT4& iColor)
	{
		mMaterialStruct.color = iColor;
	}

	DirectX::XMFLOAT4 Material::getColor() const
	{
		return mMaterialStruct.color;
	}

	bool Material::isOpaque() const
	{
		return mMaterialStruct.color.w == 1.0f;
	}

	Material::~Material()
	{
		if (mMaterialBuffer)
		{
			mMaterialBuffer->Release();
		}

		if (mVertexShader)
		{
			//mVertexShader->release();
		}

		if (mPixelShader)
		{
			//mPixelShader->release();
		}
	}

} 