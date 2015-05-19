#pragma once

#include <d3dcompiler.h>
#include <d3d11.h>
#include <cassert>

namespace GraphicEngine
{

	class VertexShader
	{

	public:

		VertexShader()
			: mVertexShader(nullptr)
		{ }

		VertexShader(const LPCWSTR& iFileName, ID3D11Device* iDevice)
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

		ID3D11VertexShader* getShader() const
		{
			return mVertexShader;
		}

		~VertexShader()
		{
			if (mVertexShader)
			{
				mVertexShader->Release();
			}
		}

	private:

		ID3D11VertexShader* mVertexShader;

	};

}