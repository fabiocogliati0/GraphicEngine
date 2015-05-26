#pragma once

#include "Material.h"
#include "Mesh.h"
#include "WorldTransform.h"

#include <memory>

namespace GraphicsEngine
{
	class Object
	{

	public:

		Object(Mesh* iMesh, Material* iMaterial, const WorldTransform& iWorldTransform);

		~Object();

		void initializeOnDevice(ID3D11Device* iDevice);

		void render(ID3D11DeviceContext* iContext);

	private:

		Mesh* mMesh;
		Material* mMaterial;
		WorldTransform mWorldTransform;

		bool mIsClipped;
		bool mIsInvisible;

	};

}