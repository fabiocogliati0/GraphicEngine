#pragma once

#include <d3dcompiler.h>
#include <d3d11.h>
#include <cassert>

namespace GraphicEngine
{

	ID3D11VertexShader* compileVertexShaders(const LPCWSTR iFileName, ID3D11Device* iDevice)
	{

		UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;

#if defined( DEBUG ) || defined( _DEBUG )
		flags |= D3DCOMPILE_DEBUG;
#endif

		// Vertex shader (compilato a build-time).
		// Caricamento del precompilato.
		ID3DBlob* vertexShaderBlob = nullptr;
		HRESULT result = D3DReadFileToBlob(iFileName, &vertexShaderBlob);
		assert(!FAILED(result));

		// Creazione dello shader.
		ID3D11VertexShader* vertexShader;
		result = iDevice->CreateVertexShader(vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), nullptr, &vertexShader);
		assert(!FAILED(result));

		if (vertexShaderBlob)
			vertexShaderBlob->Release();

		return vertexShader;

	}

	ID3D11PixelShader* compilePixelShaders(const LPCWSTR iFileName, ID3D11Device* iDevice)
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
		ID3D11PixelShader* pixelShader;
		result = iDevice->CreatePixelShader(pixelShaderBlob->GetBufferPointer(), pixelShaderBlob->GetBufferSize(), nullptr, &pixelShader);
		assert(!FAILED(result));

		if (pixelShader)
			pixelShaderBlob->Release();

		return pixelShader;
	}

}