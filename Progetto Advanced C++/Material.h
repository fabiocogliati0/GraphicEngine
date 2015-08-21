#pragma once

#include "VertexShader.h"
#include "PixelShader.h"

#include <d3d11_1.h>
#include <DirectXMath.h>


namespace GraphicsEngine
{

	class Material
	{

	public:

		Material(
			const DirectX::XMFLOAT4& iColor,
			VertexShader* iVertexShader,
			PixelShader* iPixelShader);

		void initializeOnDevice(ID3D11Device* iDevice);

		void renderSetup(ID3D11DeviceContext* iContext) const;

		void setVertexShader(VertexShader* iVertexShader);

		void setPixelShader(PixelShader* iPixelShader);

		void setColor(const DirectX::XMFLOAT4& iColor);

		DirectX::XMFLOAT4 getColor() const;

		bool isOpaque() const;

	private:

		~Material();	//Prevents stack allocation

#pragma pack(push, 16)

		struct MaterialStruct
		{
			DirectX::XMFLOAT4 color;

			MaterialStruct( const DirectX::XMFLOAT4& iColor) : color(iColor)
			{
			}

		};

#pragma pack(pop)

		MaterialStruct mMaterialStruct;
		VertexShader* mVertexShader;
		PixelShader* mPixelShader;
		ID3D11Buffer* mMaterialBuffer;

	};

}