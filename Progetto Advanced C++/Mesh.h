#pragma once

#include "Vertex.h"

#include <d3d11.h>
#include <DirectXMath.h>
#include <cassert>

namespace GraphicEngine
{
	class Mesh
	{

	public:

		Mesh() :
			mVertexBuffer(nullptr),
			mIndexBuffer(nullptr),
			mVertexCount(0),
			mIndexCount(0)
		{
		}

		Mesh(
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

			//Vertex Buffer
			bufferDesc.Usage = D3D11_USAGE_DEFAULT;
			bufferDesc.ByteWidth = sizeof(Vertex)* iVerticesNumber;
			bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bufferDesc.CPUAccessFlags = 0;
			bufferDesc.MiscFlags = 0;
			initData.pSysMem = iVertices;

			result = iDevice->CreateBuffer(&bufferDesc, &initData, &mVertexBuffer);
			assert(SUCCEEDED(result));

			//Index Buffer
			bufferDesc.Usage = D3D11_USAGE_DEFAULT;
			bufferDesc.ByteWidth = sizeof(unsigned int) * iIndicesNumber;
			bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bufferDesc.CPUAccessFlags = 0;
			bufferDesc.MiscFlags = 0;
			initData.pSysMem = iIndices;

			result = iDevice->CreateBuffer(&bufferDesc, &initData, &mIndexBuffer);
			assert(SUCCEEDED(result));

			//Input Layout	//ATTENZIONE: layoutVertex è globale, vedere se si può fare un po' un design migliore
			//result = iDevice->CreateInputLayout(layoutVertex, 1, iVertexShaderBlob->GetBufferPointer(), iVertexShaderBlob->GetBufferSize(), &mInputLayout);
		}

		void render(ID3D11DeviceContext* iContext)
		{
			UINT32 stride = sizeof(Vertex);
			UINT32 offset = 0;

			//iContext->IASetInputLayout(mInputLayout);
			iContext->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
			iContext->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
			iContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			
			iContext->DrawIndexed(mIndexCount, 0, 0);

		}

	private:

		ID3D11Buffer* mVertexBuffer;     
		ID3D11Buffer* mIndexBuffer;
		unsigned int mVertexCount;
		unsigned int mIndexCount;

		//ID3D11InputLayout* mInputLayout;

	};

}