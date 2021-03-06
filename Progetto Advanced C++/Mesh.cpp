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

	Mesh::Mesh(const Mesh& iOther) :
		mVertexBufferDesc(iOther.mVertexBufferDesc),
		mIndexBufferDesc(iOther.mIndexBufferDesc),
		mVertexInitData(iOther.mVertexInitData),
		mIndexInitData(iOther.mIndexInitData),
		mVertexBuffer(nullptr),
		mIndexBuffer(nullptr),
		mVertexCount(iOther.mVertexCount),
		mIndexCount(iOther.mIndexCount)
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

	Mesh& Mesh::operator= (const Mesh& iOther)
	{
		if (this != &iOther)
		{
			mVertexBufferDesc = iOther.mVertexBufferDesc;
			mIndexBufferDesc = iOther.mIndexBufferDesc;
			mVertexInitData = iOther.mVertexInitData;
			mIndexInitData = iOther.mIndexInitData;
			mVertexBuffer = nullptr;
			mIndexBuffer = nullptr;
			mVertexCount = iOther.mVertexCount;
			mIndexCount = iOther.mIndexCount;
		}
		return *this;
	}

	void Mesh::initializeOnDevice(ID3D11Device* iDevice)
	{

		assert(iDevice);

		if (!mVertexBuffer && !mIndexBuffer)
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

	void Mesh::renderSetup(ID3D11DeviceContext* iContext) const
	{
		
		assert(iContext);

		if (mVertexBuffer && mIndexBuffer)
		{
			UINT32 stride = sizeof(Vertex);
			UINT32 offset = 0;

			iContext->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
			iContext->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

			iContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			iContext->DrawIndexed(mIndexCount, 0, 0);
		}
	}

	
}