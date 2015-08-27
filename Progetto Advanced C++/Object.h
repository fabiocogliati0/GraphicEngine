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

		Object();

		Object(Mesh* iMesh, Material* iMaterial);

		Object(const Object& iOther);

		~Object();

		Object& operator= (const Object& iOther);

		void initializeOnDevice(ID3D11Device* iDevice);

		void render(ID3D11DeviceContext* iContext) const;

		void setVisible(bool iVisible);

		bool isVisible() const;

		bool isOpaque() const;

	private:

		Mesh* mMesh;
		Material* mMaterial;

		bool mIsHidden;

	};

}