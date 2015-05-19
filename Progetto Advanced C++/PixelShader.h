#pragma once

#include <d3d11.h>

namespace GraphicEngine
{

	class PixelShader
	{

	public:

		PixelShader();

		PixelShader(const LPCWSTR& iFileName, ID3D11Device* iDevice);

		~PixelShader();
		
		void renderSetup(ID3D11DeviceContext* iContext) const;

	private:

		ID3D11PixelShader* mPixelShader;

	};

}