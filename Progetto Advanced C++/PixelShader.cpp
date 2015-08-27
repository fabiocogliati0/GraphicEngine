#include "PixelShader.h"

#include <d3dcompiler.h>
#include <d3d11.h>
#include <cassert>

namespace GraphicsEngine
{

	PixelShader::PixelShader() :
		mFileName(L""),
		mPixelShader(nullptr)
	{
	}

	PixelShader::PixelShader(const LPCWSTR& iFileName) :
		mFileName(iFileName),
		mPixelShader(nullptr)
	{
	}

	PixelShader::PixelShader(const PixelShader& iOther) :
		mFileName(iOther.mFileName),
		mPixelShader(nullptr)
	{
	}

	PixelShader::~PixelShader()
	{
		if (mPixelShader)
		{
			mPixelShader->Release();
		}
	}

	PixelShader& PixelShader::operator = (const PixelShader& iOther)
	{
		if (this != &iOther)
		{
			mFileName = iOther.mFileName;
			mPixelShader = nullptr;
		}
	}

	void PixelShader::initOnDevice(ID3D11Device* iDevice)
	{
		assert(iDevice);

		if (!mPixelShader)
		{
			//Load precompiled shaders
			ID3DBlob* pixelShaderBlob = nullptr;
			HRESULT result = D3DReadFileToBlob(mFileName, &pixelShaderBlob);
			assert(SUCCEEDED(result));

			//Create Shader and link to the device
			result = iDevice->CreatePixelShader(pixelShaderBlob->GetBufferPointer(), pixelShaderBlob->GetBufferSize(), nullptr, &mPixelShader);
			assert(SUCCEEDED(result));

			//Release Shader file
			if (pixelShaderBlob)
			{
				pixelShaderBlob->Release();
			}
		}

	}

	void PixelShader::renderSetup(ID3D11DeviceContext* iContext) const
	{
		assert(iContext);

		if (mPixelShader)
		{
			iContext->PSSetShader(mPixelShader, nullptr, 0);
		}
	}

}