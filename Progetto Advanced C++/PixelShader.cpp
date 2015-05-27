#include "PixelShader.h"

#include <d3dcompiler.h>
#include <d3d11.h>
#include <cassert>

namespace GraphicsEngine
{

	PixelShader::PixelShader(const LPCWSTR& iFileName) :
		mFileName(iFileName),
		mPixelShader(nullptr)
	{
	}

	void PixelShader::initOnDevice(ID3D11Device* iDevice)
	{
		if (!mPixelShader && iDevice)
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
		if (mPixelShader && iContext)
		{
			iContext->PSSetShader(mPixelShader, nullptr, 0);
		}
	}

	PixelShader::~PixelShader()
	{
		if (mPixelShader)
		{
			mPixelShader->Release();
		}
	}

}