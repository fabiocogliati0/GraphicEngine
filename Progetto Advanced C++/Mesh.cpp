#include "Mesh.h"

#include "Vertex.h"

#include <d3d11.h>
#include <DirectXMath.h>
#include <cassert>

namespace GraphicEngine
{

	Mesh::Mesh() :
		mVertexBuffer(nullptr),
		mIndexBuffer(nullptr),
		mVertexCount(0),
		mIndexCount(0)
	{
	}

	Mesh::Mesh(
		Vertex* iVertices,
		unsigned int iVerticesNumber,
		unsigned int* iIndices,
		unsigned int iIndicesNumber,
		ID3D11Device* iDevice
		)
	{
		D3D11_BUFFER_DESC bufferDesc;
		D3D11_SUBRESOURCE_DATA initData;
		HRESULT result;

		//Create Vertex Buffer
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.ByteWidth = sizeof(Vertex)* iVerticesNumber;
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = 0;
		initData.pSysMem = iVertices;

		result = iDevice->CreateBuffer(&bufferDesc, &initData, &mVertexBuffer);
		assert(SUCCEEDED(result));

		//Create Index Buffer
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.ByteWidth = sizeof(unsigned int)* iIndicesNumber;
		bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = 0;
		initData.pSysMem = iIndices;

		result = iDevice->CreateBuffer(&bufferDesc, &initData, &mIndexBuffer);
		assert(SUCCEEDED(result));
	}

	Mesh::~Mesh()
	{
		if (mVertexBuffer)
			mVertexBuffer->Release();

		if (mIndexBuffer)
			mIndexBuffer->Release();
	}

	void Mesh::render(ID3D11DeviceContext* iContext)
	{
		UINT32 stride = sizeof(Vertex);
		UINT32 offset = 0;

		iContext->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
		iContext->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
		iContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		iContext->DrawIndexed(mIndexCount, 0, 0);
	}

}