#pragma once

#include "Vertex.h"

#include <d3d11.h>

namespace GraphicEngine
{
	class Mesh
	{

	public:

		Mesh();

		Mesh(
			Vertex* iVertices,
			unsigned int iVerticesNumber,
			unsigned int* iIndices,
			unsigned int iIndicesNumber,
			ID3D11Device* iDevice);

		~Mesh();

		void render(ID3D11DeviceContext* iContext);

	private:

		ID3D11Buffer* mVertexBuffer;     
		ID3D11Buffer* mIndexBuffer;
		unsigned int mVertexCount;
		unsigned int mIndexCount;

	};

}