#include "Object.h"

#include "Mesh.h"
#include "Material.h"
#include "WorldTransform.h"

namespace GraphicEngine
{
	Object::Object(Mesh* iMesh, Material* iMaterial, WorldTransform* iWorldTransform)
		: mMesh(iMesh), mMaterial(iMaterial), mWorldTransform(iWorldTransform), mIsClipped(false), mIsInvisible(false)
	{
	}

	void Object::initializeOnDevice(ID3D11Device* iDevice)
	{
		if (mMaterial && mWorldTransform && mMesh && iDevice)
		{
			mMaterial->initializeOnDevice(iDevice);
			mWorldTransform->initializeOnDevice(iDevice);
			mMesh->initializeOnDevice(iDevice);
		}
	}

	void Object::render(ID3D11DeviceContext* iContext)
	{
		if (mMaterial && mWorldTransform && mMesh && iContext)
		{
			mMaterial->renderSetup(iContext);
			mWorldTransform->renderSetup(iContext);

			mMesh->render(iContext);
		}
	}

}

