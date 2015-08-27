#include "VertexShader.h"

#include <d3dcompiler.h>
#include <d3d11.h>
#include <cassert>

namespace GraphicsEngine
{
	
	VertexShader::VertexShader() :
		mFileName(L""),
		mInputLayoutDesc(nullptr),
		mInputLayoutSize(0),
		mVertexShader(nullptr),
		mInputLayout(nullptr)
	{
	}

	VertexShader::VertexShader(
		const LPCWSTR& iFileName,
		const D3D11_INPUT_ELEMENT_DESC* iInputLayoutDesc,
		unsigned int iInputLayoutSize) :
			mFileName(iFileName),
			mInputLayoutDesc(iInputLayoutDesc),
			mInputLayoutSize(iInputLayoutSize),
			mVertexShader(nullptr),
			mInputLayout(nullptr)
	{
	}

	VertexShader::VertexShader(const VertexShader& iOther) :
		mFileName(iOther.mFileName),
		mInputLayoutDesc(iOther.mInputLayoutDesc),
		mInputLayoutSize(iOther.mInputLayoutSize),
		mVertexShader(nullptr),
		mInputLayout(nullptr)
	{
	}

	VertexShader& VertexShader::operator = (const VertexShader& iOther)
	{
		if (this != &iOther)
		{
			mFileName = iOther.mFileName;
			mInputLayoutDesc = iOther.mInputLayoutDesc;
			mInputLayoutSize = iOther.mInputLayoutSize;
			mVertexShader = nullptr;
			mInputLayout = nullptr;
		}
		return *this;
	}

	void VertexShader::initOnDevice(ID3D11Device* iDevice)
	{

		assert(iDevice);

		if (!mVertexShader)
		{
			//Load precompiled shaders
			ID3DBlob* vertexShaderBlob = nullptr;
			HRESULT result = D3DReadFileToBlob(mFileName, &vertexShaderBlob);
			assert(SUCCEEDED(result));

			//Create Shader and link to the device
			result = iDevice->CreateVertexShader(vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), nullptr, &mVertexShader);
			assert(SUCCEEDED(result));

			//Create input layout
			result = iDevice->CreateInputLayout(
				mInputLayoutDesc,
				mInputLayoutSize,
				vertexShaderBlob->GetBufferPointer(),
				vertexShaderBlob->GetBufferSize(),
				&mInputLayout);

			//Release Shader file
			if (vertexShaderBlob)
			{
				vertexShaderBlob->Release();
			}
		}
	}

	void VertexShader::renderSetup(ID3D11DeviceContext* iContext) const
	{
		assert(iContext);

		if (mInputLayout)
			iContext->IASetInputLayout(mInputLayout);

		if (mVertexShader)
			iContext->VSSetShader(mVertexShader, nullptr, 0);
	}

	VertexShader::~VertexShader()
	{
		if (mVertexShader)
		{
			mVertexShader->Release();
		}

		if (mInputLayout)
		{
			mInputLayout->Release();
		}
	}

}