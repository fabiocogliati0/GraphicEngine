//#pragma once
//
//#include "Material.h"
//#include "Mesh.h"
//#include "Trasformations.h"
//
//#include <memory>
//
//namespace GraphicEngine
//{
//	class Object
//	{
//
//	public:
//
//		Object(const Mesh& iMesh, Material* iMaterial, const TransformationsStruct& iTransformations, ID3D11Device* iDevice)
//			: mMesh(iMesh), mMaterial(iMaterial), mTransformations(iTransformations)
//		{
//			D3D11_BUFFER_DESC bufferDesc;
//			D3D11_SUBRESOURCE_DATA initData;
//			HRESULT result;
//
//			//Transform Buffer
//			bufferDesc.Usage = D3D11_USAGE_DEFAULT;
//			bufferDesc.ByteWidth = sizeof(TransformationsStruct);
//			bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
//			bufferDesc.CPUAccessFlags = 0;
//			bufferDesc.MiscFlags = 0;
//			initData.pSysMem = &iTransformations;
//			result = iDevice->CreateBuffer(&bufferDesc, &initData, &mTransformationsBuffer);
//			assert(SUCCEEDED(result));
//			
//		}
//
//		void render(ID3D11DeviceContext* iContext)
//		{
//			mMaterial->renderSetup(iContext);
//
//			//devo settare le trasformazioni
//
//			mMesh.render(iContext);
//
//		}
//
//	private:
//
//		Mesh mMesh;
//		
//		Material* mMaterial;
//
//		//Material* mMaterial;		//pensare se condividerlo tra più oggetti
//		TransformationsStruct mTransformations;
//
//		ID3D11Buffer* mTransformationsBuffer;
//
//		bool mIsClipped;
//		bool mIsInvisible;
//
//	};
//
//}