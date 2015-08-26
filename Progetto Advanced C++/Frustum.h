#pragma once

#include <DirectXMath.h>

namespace GraphicsEngine
{

	//Forward declarations
	class AABB;


	class Frustum
	{
	
	public:
		
		Frustum();
		
		Frustum(const Frustum&);
		
		~Frustum();

		void ConstructFrustum(const DirectX::XMMATRIX& iViewMatrix, const DirectX::XMMATRIX& iProjectionMatrix);

		bool CheckAABB(const AABB& iAABB);

	
	private:
		
		DirectX::XMFLOAT4 mFrustumPlanes[6];
	};

}
