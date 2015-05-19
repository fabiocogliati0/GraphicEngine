#include "VertexShader.h"

#include <d3dcompiler.h>
#include <d3d11.h>
#include <cassert>

namespace GraphicEngine
{
	
	VertexShader::VertexShader() : mVertexShader(nullptr) 
	{
	}

	VertexShader::VertexShader(const LPCWSTR& iFileName, ID3D11Device* iDevice)
	{
		UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;

#if defined( DEBUG ) || defined( _DEBUG )
		flags |= D3DCOMPILE_DEBUG;
#endif

		// Vertex shader (compilato a build-time).
		// Caricamento del precompilato.
		ID3DBlob* pixelShaderBlob = nullptr;
		HRESULT result = D3DReadFileToBlob(iFileName, &pixelShaderBlob);
		assert(!FAILED(result));

		// Creazione dello shader.
		result = iDevice->CreateVertexShader(pixelShaderBlob->GetBufferPointer(), pixelShaderBlob->GetBufferSize(), nullptr, &mVertexShader);
		assert(!FAILED(result));

		if (pixelShaderBlob)
			pixelShaderBlob->Release();
	}

	VertexShader::~VertexShader()
	{
		if (mVertexShader)
		{
			mVertexShader->Release();
		}
	}

	void VertexShader::renderSetup(ID3D11DeviceContext* iContext) const
	{
		iContext->VSSetShader(mVertexShader, nullptr, 0);
	}

}