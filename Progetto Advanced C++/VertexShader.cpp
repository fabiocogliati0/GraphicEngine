#include "VertexShader.h"

#include <d3dcompiler.h>
#include <d3d11.h>
#include <cassert>

namespace GraphicEngine
{
	
	VertexShader::VertexShader() 
		: mVertexShader(nullptr), mInputLayout(nullptr)
	{
	}

	VertexShader::VertexShader(
		const LPCWSTR& iFileName,
		const D3D11_INPUT_ELEMENT_DESC* iInputLayoutDesc, 
		unsigned int iInputLayoutSize,
		ID3D11Device* iDevice)
	{
		UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;

#if defined( DEBUG ) || defined( _DEBUG )
		flags |= D3DCOMPILE_DEBUG;
#endif

		//Load precompiled shaders
		ID3DBlob* vertexShaderBlob = nullptr;
		HRESULT result = D3DReadFileToBlob(iFileName, &vertexShaderBlob);
		assert(!FAILED(result));

		//Create Shader and link to the device
		result = iDevice->CreateVertexShader(vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), nullptr, &mVertexShader);
		assert(!FAILED(result));

		//Create input layout
		result = iDevice->CreateInputLayout(iInputLayoutDesc, iInputLayoutSize, vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), &mInputLayout);

		//Release Shader file
		if (vertexShaderBlob)
			vertexShaderBlob->Release();
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
		//TODO: i puntatori a inputLayout e vertexShader possono essere nulli, che fare?
		iContext->IASetInputLayout(mInputLayout);
		iContext->VSSetShader(mVertexShader, nullptr, 0);
	}

}