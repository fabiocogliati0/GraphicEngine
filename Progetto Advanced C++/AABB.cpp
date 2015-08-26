#include "AABB.h"

#include <DirectXMath.h>
#include <cmath>
#include <cfloat>

namespace GraphicsEngine
{

	AABB::AABB()
	{
	}

	AABB::AABB(const DirectX::XMFLOAT3* iVertices, int iVerticesNumber) :
		mMin(DirectX::XMFLOAT3(FLT_MAX,FLT_MAX,FLT_MAX)),
		mMax(DirectX::XMFLOAT3(FLT_MIN, FLT_MIN, FLT_MIN))
	{
		for (int i = 0; i < iVerticesNumber; ++i)
		{
			mMin.x = fmin(mMin.x, iVertices[i].x);
			mMin.y = fmin(mMin.y, iVertices[i].y);
			mMin.z = fmin(mMin.z, iVertices[i].z);

			mMax.x = fmax(mMax.x, iVertices[i].x);
			mMax.y = fmax(mMax.y, iVertices[i].y);
			mMax.z = fmax(mMax.z, iVertices[i].z);
		}
	}

	const DirectX::XMFLOAT3& AABB::getMin() const
	{
		return mMin;
	}

	const DirectX::XMFLOAT3& AABB::getMax() const
	{
		return mMax;
	}

}