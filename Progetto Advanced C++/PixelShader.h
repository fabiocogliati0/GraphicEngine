#pragma once

#include <d3d11.h>

namespace GraphicsEngine
{

	class PixelShader
	{

	public:

		PixelShader(const LPCWSTR& iFileName);

		~PixelShader();
		
		void initOnDevice(ID3D11Device* iDevice);

		void renderSetup(ID3D11DeviceContext* iContext) const;

		void release();

	private:

		LPCWSTR mFileName;
		ID3D11PixelShader* mPixelShader;

	};

}