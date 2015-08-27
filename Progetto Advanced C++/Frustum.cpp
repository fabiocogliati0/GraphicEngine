#include "Frustum.h"
#include "AABB.h"
#include "WorldTransform.h"

#include <DirectXMath.h>
#include <cmath>

namespace GraphicsEngine
{

	Frustum::Frustum()
	{
	}

	void Frustum::ConstructFrustum(const DirectX::XMMATRIX& iViewMatrix, const DirectX::XMMATRIX& iProjectionMatrix)
	{

		DirectX::XMFLOAT4X4 viewProj;
		DirectX::XMStoreFloat4x4(&viewProj, DirectX::XMMatrixMultiply(iViewMatrix, iProjectionMatrix));

		// Left Frustum Plane
		// Add first column of the matrix to the fourth column
		mFrustumPlanes[0].x = viewProj._14 + viewProj._11;
		mFrustumPlanes[0].y = viewProj._24 + viewProj._21;
		mFrustumPlanes[0].z = viewProj._34 + viewProj._31;
		mFrustumPlanes[0].w = viewProj._44 + viewProj._41;

		// Right Frustum Plane
		// Subtract first column of matrix from the fourth column
		mFrustumPlanes[1].x = viewProj._14 - viewProj._11;
		mFrustumPlanes[1].y = viewProj._24 - viewProj._21;
		mFrustumPlanes[1].z = viewProj._34 - viewProj._31;
		mFrustumPlanes[1].w = viewProj._44 - viewProj._41;

		// Top Frustum Plane
		// Subtract second column of matrix from the fourth column
		mFrustumPlanes[2].x = viewProj._14 - viewProj._12;
		mFrustumPlanes[2].y = viewProj._24 - viewProj._22;
		mFrustumPlanes[2].z = viewProj._34 - viewProj._32;
		mFrustumPlanes[2].w = viewProj._44 - viewProj._42;

		// Bottom Frustum Plane
		// Add second column of the matrix to the fourth column
		mFrustumPlanes[3].x = viewProj._14 + viewProj._12;
		mFrustumPlanes[3].y = viewProj._24 + viewProj._22;
		mFrustumPlanes[3].z = viewProj._34 + viewProj._32;
		mFrustumPlanes[3].w = viewProj._44 + viewProj._42;

		// Near Frustum Plane
		// We could add the third column to the fourth column to get the near plane,
		// but we don't have to do this because the third column IS the near plane
		mFrustumPlanes[4].x = viewProj._13;
		mFrustumPlanes[4].y = viewProj._23;
		mFrustumPlanes[4].z = viewProj._33;
		mFrustumPlanes[4].w = viewProj._43;

		// Far Frustum Plane
		// Subtract third column of matrix from the fourth column
		mFrustumPlanes[5].x = viewProj._14 - viewProj._13;
		mFrustumPlanes[5].y = viewProj._24 - viewProj._23;
		mFrustumPlanes[5].z = viewProj._34 - viewProj._33;
		mFrustumPlanes[5].w = viewProj._44 - viewProj._43;

		// Normalize Planes
		for (int i = 0; i < 6; ++i)
		{
			float length = sqrt((mFrustumPlanes[i].x * mFrustumPlanes[i].x) +
				(mFrustumPlanes[i].y * mFrustumPlanes[i].y) + 
				(mFrustumPlanes[i].z * mFrustumPlanes[i].z));

			mFrustumPlanes[i].x /= length;
			mFrustumPlanes[i].y /= length;
			mFrustumPlanes[i].z /= length;
			mFrustumPlanes[i].w /= length;
		}

	}

	bool Frustum::CheckAABB(const AABB& iAABB, const WorldTransform& iObjectTransform) const
	{

		bool cull = false;

		const DirectX::XMFLOAT3 min = iObjectTransform.getGlobalPosition(iAABB.getMin());
		const DirectX::XMFLOAT3 max = iObjectTransform.getGlobalPosition(iAABB.getMax());

		for (int i = 0; i < 6; ++i)
		{
			DirectX::XMVECTOR planeNormal = DirectX::XMVectorSet(mFrustumPlanes[i].x, mFrustumPlanes[i].y, mFrustumPlanes[i].z, 0.0f);
			float planeConstant = mFrustumPlanes[i].w;

			DirectX::XMFLOAT3 axisVert;

			if (mFrustumPlanes[i].x < 0.0f)
				axisVert.x = min.x;
			else
				axisVert.x = max.x;
			
			if (mFrustumPlanes[i].y < 0.0f)
				axisVert.y = min.y;
			else
				axisVert.y = max.y;

			if (mFrustumPlanes[i].z < 0.0f)
				axisVert.z = min.z;
			else
				axisVert.z = max.z;
			
			if (DirectX::XMVectorGetX(DirectX::XMVector3Dot(planeNormal, DirectX::XMLoadFloat3(&axisVert))) + planeConstant < 0.0f)
			{
				cull = true;
				break;
			}

		}

		return cull;

	}

}