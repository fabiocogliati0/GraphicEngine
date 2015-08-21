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

		Object();	//TODO: ho dovuto aggiungerlo per poter mettere un array sullo stack

		Object(Mesh* iMesh, Material* iMaterial, const WorldTransform& iWorldTransform);

		~Object();

		void initializeOnDevice(ID3D11Device* iDevice);

		void render(ID3D11DeviceContext* iContext);

		void translate(float iX, float iY, float iZ);

		void translate(float iX, float iY, float iZ, ID3D11DeviceContext* iContext);

		void setVisible(bool iVisible);

		bool isVisible() const;

		bool isOpaque() const;

	private:

		Mesh* mMesh;
		Material* mMaterial;
		WorldTransform mWorldTransform;

		bool mIsClipped;
		bool mIsHidden;

	};

}