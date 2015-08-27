#pragma once

#include <d3d11.h>

namespace GraphicsEngine
{

	class PixelShader 
	{

	public:

		PixelShader();

		PixelShader(const LPCWSTR& iFileName);

		PixelShader(const PixelShader& iOther);

		~PixelShader();

		PixelShader& operator=(const PixelShader& iOther);
		
		void initOnDevice(ID3D11Device* iDevice);

		void renderSetup(ID3D11DeviceContext* iContext) const;

	private:

		LPCWSTR mFileName;
		ID3D11PixelShader* mPixelShader;

	};

}