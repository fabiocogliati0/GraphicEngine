#pragma once

#include "Material.h"
#include "Mesh.h"
#include "WorldTransform.h"

#include <memory>

namespace GraphicEngine
{
	class Object
	{

	public:

		Object(Mesh* iMesh, Material* iMaterial, WorldTransform* iTransformations)
			: mMesh(iMesh), mMaterial(iMaterial), mTransformations(iTransformations)
		{	
		}

		void render(ID3D11DeviceContext* iContext)
		{
			mMaterial->renderSetup(iContext);
			mTransformations->renderSetup(iContext);
			
			mMesh->	render(iContext);
		}

	private:

		Mesh* mMesh;
		Material* mMaterial;
		WorldTransform* mTransformations;

		bool mIsClipped;
		bool mIsInvisible;

	};

}