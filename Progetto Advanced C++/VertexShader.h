#pragma once

#include <d3d11.h>

namespace GraphicEngine
{

	class VertexShader
	{

	public:

		VertexShader();

		VertexShader(const LPCWSTR& iFileName, ID3D11Device* iDevice);

		~VertexShader();

		void renderSetup(ID3D11DeviceContext* iContext) const;

	private:

		ID3D11VertexShader* mVertexShader;

	};

}