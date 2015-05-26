#pragma once

#include "RefCountBase.h"

#include <d3d11.h>

namespace GraphicsEngine
{

	class PixelShader : public RefCountBase 
	{

	public:

		PixelShader(const LPCWSTR& iFileName);
		
		void initOnDevice(ID3D11Device* iDevice);

		void renderSetup(ID3D11DeviceContext* iContext) const;

	private:

		~PixelShader();	//prevents stack allocation

		LPCWSTR mFileName;
		ID3D11PixelShader* mPixelShader;

	};

}