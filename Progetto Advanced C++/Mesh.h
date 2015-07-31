#pragma once

#include "Vertex.h"

#include <d3d11.h>

namespace GraphicsEngine
{
	class Mesh 
	{

	public:

		Mesh();

		Mesh(
			Vertex* iVertices,
			unsigned int iVerticesNumber,
			unsigned int* iIndices,
			unsigned int iIndicesNumber);

		void initializeOnDevice(ID3D11Device* iDevice);

		void render(ID3D11DeviceContext* iContext);

	private:

		~Mesh();	//prevents stack allocation

		ID3D11Buffer* mVertexBuffer;     
		ID3D11Buffer* mIndexBuffer;

		D3D11_BUFFER_DESC mVertexBufferDesc;
		D3D11_BUFFER_DESC mIndexBufferDesc;
		D3D11_SUBRESOURCE_DATA mVertexInitData;
		D3D11_SUBRESOURCE_DATA mIndexInitData;

		unsigned int mVertexCount;
		unsigned int mIndexCount;

	};

}