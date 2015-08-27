#include "Material.h"

#include "VertexShader.h"
#include "PixelShader.h"

#include <d3d11_1.h>
#include <DirectXMath.h>


namespace GraphicsEngine
{

	Material::Material() :
		mMaterialStruct(),
		mVertexShader(nullptr),
		mPixelShader(nullptr),
		mMaterialBuffer(nullptr)
	{
	}

	Material::Material(
		const DirectX::XMFLOAT4& iColor,
		VertexShader* iVertexShader,
		PixelShader* iPixelShader) :
			mMaterialStruct(iColor),
			mVertexShader(iVertexShader),
			mPixelShader(iPixelShader),
			mMaterialBuffer(nullptr)
	{
	}

	Material::Material(const Material& iOther) :
		mMaterialStruct(iOther.mMaterialStruct),
		mVertexShader(iOther.mVertexShader),
		mPixelShader(iOther.mPixelShader),
		mMaterialBuffer(nullptr)
	{
	}

	Material::~Material()
	{
		if (mMaterialBuffer)
		{
			mMaterialBuffer->Release();
		}
	}

	Material& Material::operator= (const Material& iOther)
	{
		if (this != &iOther)
		{
			mMaterialStruct = iOther.mMaterialStruct;
			mVertexShader = iOther.mVertexShader;
			mPixelShader = iOther.mPixelShader;
			mMaterialBuffer = nullptr;
		}
		return *this;
	}

	void Material::initializeOnDevice(ID3D11Device* iDevice)
	{

		assert(iDevice);

		if (!mMaterialBuffer)
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

			if (mVertexShader)
				mVertexShader->initOnDevice(iDevice);

			if (mPixelShader)
				mPixelShader->initOnDevice(iDevice);
		}
	}

	void Material::renderSetup(ID3D11DeviceContext* iContext) const
	{

		assert(iContext);
		
		if (mVertexShader)
			mVertexShader->renderSetup(iContext);

		if (mPixelShader)
			mPixelShader->renderSetup(iContext);

		if (mMaterialBuffer)
			iContext->VSSetConstantBuffers(2, 1, &mMaterialBuffer);
	}

	bool Material::isOpaque() const
	{
		return mMaterialStruct.color.w == 1.0f;
	}

} 