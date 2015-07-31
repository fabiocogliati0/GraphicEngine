#pragma once

#include <d3d11.h>

namespace GraphicsEngine
{

	class VertexShader
	{

	public:

		VertexShader(
			const LPCWSTR& iFileName,
			const D3D11_INPUT_ELEMENT_DESC* iInputLayoutDesc,
			unsigned int iInputLayoutSize);

		void initOnDevice(ID3D11Device* iDevice);

		void renderSetup(ID3D11DeviceContext* iContext) const;

	private:

		~VertexShader(); //Prevents stack allocation

		LPCWSTR mFileName;
		const D3D11_INPUT_ELEMENT_DESC* mInputLayoutDesc;
		unsigned int mInputLayoutSize;
		ID3D11VertexShader* mVertexShader;
		ID3D11InputLayout* mInputLayout;

	};

}