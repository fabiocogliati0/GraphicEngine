#pragma once

#include <d3d11.h>

namespace GraphicEngine
{

	class VertexShader
	{

	public:

		VertexShader();

		VertexShader(
			const LPCWSTR& iFileName,
			const D3D11_INPUT_ELEMENT_DESC* iInputLayout,
			unsigned int iInputLayoutSize,
			ID3D11Device* iDevice);

		~VertexShader();

		void renderSetup(ID3D11DeviceContext* iContext) const;

	private:

		ID3D11VertexShader* mVertexShader;
		ID3D11InputLayout* mInputLayout;

	};

}