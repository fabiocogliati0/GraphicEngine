#pragma once

#include <d3dcompiler.h>
#include <d3d11.h>
#include <cassert>

namespace GraphicEngine
{
	
	class PixelShader
	{

	public:

		PixelShader() 
			: mPixelShader(nullptr)
		{ }

		PixelShader(const LPCWSTR& iFileName, ID3D11Device* iDevice)
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
			result = iDevice->CreatePixelShader(pixelShaderBlob->GetBufferPointer(), pixelShaderBlob->GetBufferSize(), nullptr, &mPixelShader);
			assert(!FAILED(result));

			if (pixelShaderBlob)
				pixelShaderBlob->Release();
		}

		ID3D11PixelShader* getShader() const
		{
			return mPixelShader;
		}

		~PixelShader()
		{
			if (mPixelShader)
			{
				mPixelShader->Release();
			}
		}
		
	private:

		ID3D11PixelShader* mPixelShader;

	};
	
}