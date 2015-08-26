#include "Object.h"

#include "Mesh.h"
#include "Material.h"
#include "WorldTransform.h"

namespace GraphicsEngine
{

	Object::Object() :
		mMesh(nullptr), mMaterial(nullptr)
	{
	}

	Object::Object(Mesh* iMesh, Material* iMaterial) :
		mMesh(iMesh), mMaterial(iMaterial), mIsClipped(false), mIsHidden(false)
	{
		//mMesh->grab();
		//mMaterial->grab();
	}

	Object::~Object()
	{
		if (mMaterial)
		{
			//mMaterial->release();
		}
		
		if (mMesh)
		{
			//mMesh->release();
		}
	}

	void Object::initializeOnDevice(ID3D11Device* iDevice)
	{
		if (mMaterial && mMesh && iDevice)
		{
			mMaterial->initializeOnDevice(iDevice);
			mMesh->initializeOnDevice(iDevice);
		}
	}

	void Object::render(ID3D11DeviceContext* iContext)
	{
		if (!mIsHidden && mMaterial && mMesh && iContext)
		{
			mMaterial->renderSetup(iContext);
			mMesh->render(iContext);
		}
	}
	
	void Object::setVisible(bool iVisible)
	{
		mIsHidden = !iVisible;
	}

	bool Object::isVisible() const
	{
		return !mIsHidden;
	}

	bool Object::isOpaque() const
	{
		return mMaterial->isOpaque();
	}

}