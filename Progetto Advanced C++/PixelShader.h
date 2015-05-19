#pragma once

#include <d3dcompiler.h>
#include <d3d11.h>
#include <cassert>

namespace GraphicEngine
{

	class PixelShader
	{

	public:

		PixelShader();

		PixelShader(const LPCWSTR& iFileName, ID3D11Device* iDevice);

		ID3D11PixelShader* getShader() const;

		~PixelShader();

	private:

		ID3D11PixelShader* mPixelShader;

	};

}