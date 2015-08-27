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

		Material();

		Material(
			const DirectX::XMFLOAT4& iColor,
			VertexShader* iVertexShader,
			PixelShader* iPixelShader);

		Material(const Material& iOther);

		~Material();

		Material& operator=(const Material& iOther);

		void initializeOnDevice(ID3D11Device* iDevice);

		void renderSetup(ID3D11DeviceContext* iContext) const;

		bool isOpaque() const;

	private:

#pragma pack(push, 16)

		struct MaterialStruct
		{
			DirectX::XMFLOAT4 color;

			MaterialStruct() :
				color(DirectX::XMFLOAT4())
			{
			}

			MaterialStruct( const DirectX::XMFLOAT4& iColor) :
				color(iColor)
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