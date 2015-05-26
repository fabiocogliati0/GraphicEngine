#pragma once

#include "Object.h"
#include "Lights.h"

#include <vector>

namespace GraphicsEngine
{

	class World
	{

	public:

		World()
		{
		}

		/*World(const Camera iCamera) :
			mObjects(),
			mCamera(iCamera)
		{
		}*/

		void sinitializeOnDevice(ID3D11Device* iDevice)
		{
			mLights.initializeOnDevice(iDevice);
		}

		void addBody(Object* iObject, ID3D11Device* iDevice)
		{
			mObjects.push_back(iObject);
			iObject->initializeOnDevice(iDevice);
		}

		void render(ID3D11DeviceContext* iContext) const
		{
			for (unsigned int i = 0; i < mObjects.size(); ++i)
			{
				mObjects[i]->render(iContext);
			}
		}


	private:

		std::vector<Object*> mObjects;
		//Lights* lights;
		//Camera mCamera;
		Lights mLights;

	};

}