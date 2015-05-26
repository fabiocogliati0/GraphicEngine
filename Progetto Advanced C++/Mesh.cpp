#include "Mesh.h"

#include "Vertex.h"

#include <d3d11.h>
#include <DirectXMath.h>
#include <cassert>

namespace GraphicsEngine
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
		unsigned int iIndicesNumber) :
			mVertexCount(iVerticesNumber),
			mIndexCount(iIndicesNumber)
	{	
		//Create Vertex Buffer desc
		mVertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		mVertexBufferDesc.ByteWidth = sizeof(Vertex)* iVerticesNumber;
		mVertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		mVertexBufferDesc.CPUAccessFlags = 0;
		mVertexBufferDesc.MiscFlags = 0;
		mVertexInitData.pSysMem = iVertices;

		//Create Index Buffer desc
		mIndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		mIndexBufferDesc.ByteWidth = sizeof(unsigned int)* iIndicesNumber;
		mIndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		mIndexBufferDesc.CPUAccessFlags = 0;
		mIndexBufferDesc.MiscFlags = 0;
		mIndexInitData.pSysMem = iIndices;	
	}

	Mesh::~Mesh()
	{
		if (mVertexBuffer)
		{
			mVertexBuffer->Release();
		}

		if (mIndexBuffer)
		{
			mIndexBuffer->Release();
		}
	}

	void Mesh::initializeOnDevice(ID3D11Device* iDevice)
	{
		if (!mVertexBuffer && !mIndexBuffer && iDevice)
		{
			HRESULT result;

			//Create vertex buffer
			result = iDevice->CreateBuffer(&mVertexBufferDesc, &mVertexInitData, &mVertexBuffer);
			assert(SUCCEEDED(result));

			//Create index buffer
			result = iDevice->CreateBuffer(&mIndexBufferDesc, &mIndexInitData, &mIndexBuffer);
			assert(SUCCEEDED(result));
		}
	}

	void Mesh::render(ID3D11DeviceContext* iContext)
	{
		if (mVertexBuffer && mIndexBuffer && iContext)
		{
			UINT32 stride = sizeof(Vertex);
			UINT32 offset = 0;

			iContext->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
			iContext->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
			iContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			iContext->DrawIndexed(mIndexCount, 0, 0);
		}
	}

}