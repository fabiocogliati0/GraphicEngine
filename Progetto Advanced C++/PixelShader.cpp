#include "PixelShader.h"

#include <d3dcompiler.h>
#include <d3d11.h>
#include <cassert>

namespace GraphicEngine
{

	PixelShader::PixelShader() : mPixelShader(nullptr)
	{
	}

	PixelShader::PixelShader(const LPCWSTR& iFileName, ID3D11Device* iDevice)
	{
		UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;

#if defined( DEBUG ) || defined( _DEBUG )
		flags |= D3DCOMPILE_DEBUG;
#endif

		//Load precompiled shaders
		ID3DBlob* pixelShaderBlob = nullptr;
		HRESULT result = D3DReadFileToBlob(iFileName, &pixelShaderBlob);
		assert(!FAILED(result));

		//Create Shader and link to the device
		result = iDevice->CreatePixelShader(pixelShaderBlob->GetBufferPointer(), pixelShaderBlob->GetBufferSize(), nullptr, &mPixelShader);
		assert(!FAILED(result));

		//Release Shader file
		if (pixelShaderBlob)
			pixelShaderBlob->Release();
	}

	ID3D11PixelShader* PixelShader::getShader() const
	{
		return mPixelShader;
	}

	PixelShader::~PixelShader()
	{
		if (mPixelShader)
		{
			mPixelShader->Release();
		}
	}


}