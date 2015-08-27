#pragma once

#include <DirectXMath.h>

namespace GraphicsEngine
{

	//Forward declarations
	class AABB;
	class WorldTransform;


	class Frustum
	{
	
	public:
		
		Frustum();

		void ConstructFrustum(const DirectX::XMMATRIX& iViewMatrix, const DirectX::XMMATRIX& iProjectionMatrix);

		bool CheckAABB(const AABB& iAABB, const WorldTransform& iObjectTransform) const;

	
	private:
		
		DirectX::XMFLOAT4 mFrustumPlanes[6];
	};

}
