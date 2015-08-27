#include "Object.h"

#include "Mesh.h"
#include "Material.h"
#include "WorldTransform.h"

#include <cassert>

namespace GraphicsEngine
{

	Object::Object() :
		mMesh(nullptr), mMaterial(nullptr), mIsHidden(true)
	{
	}

	Object::Object(Mesh* iMesh, Material* iMaterial) :
		mMesh(iMesh), mMaterial(iMaterial), mIsHidden(false)
	{
	}

	Object::Object(const Object& iOther) :
		mMesh(iOther.mMesh), mMaterial(iOther.mMaterial), mIsHidden(iOther.mIsHidden)
	{
	}

	Object::~Object()
	{
	}

	Object& Object::operator = (const Object& iOther)
	{
		if (this != &iOther)
		{
			mMesh = iOther.mMesh;
			mMaterial = iOther.mMaterial;
			mIsHidden = iOther.mIsHidden;
		}
		return *this;
	}

	void Object::initializeOnDevice(ID3D11Device* iDevice)
	{
		assert(iDevice);

		if (mMaterial)
			mMaterial->initializeOnDevice(iDevice);

		if (mMesh)
			mMesh->initializeOnDevice(iDevice);

	}

	void Object::render(ID3D11DeviceContext* iContext) const
	{
		assert(iContext);

		if (!mIsHidden)
		{
			if (mMaterial)
				mMaterial->renderSetup(iContext);
			
			if (mMesh)
				mMesh->renderSetup(iContext);
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