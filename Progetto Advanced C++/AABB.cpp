#include "AABB.h"
#include "Vertex.h"

#include <DirectXMath.h>
#include <cmath>
#include <cfloat>

namespace GraphicsEngine
{

	AABB::AABB()
	{
	}

	AABB::AABB(const Vertex* iVertices, int iVerticesNumber) :
		mMin(DirectX::XMFLOAT3(FLT_MAX,FLT_MAX,FLT_MAX)),
		mMax(DirectX::XMFLOAT3(FLT_MIN, FLT_MIN, FLT_MIN))
	{
		for (int i = 0; i < iVerticesNumber; ++i)
		{
			mMin.x = fmin(mMin.x, iVertices[i].position.x);
			mMin.y = fmin(mMin.y, iVertices[i].position.y);
			mMin.z = fmin(mMin.z, iVertices[i].position.z);

			mMax.x = fmax(mMax.x, iVertices[i].position.x);
			mMax.y = fmax(mMax.y, iVertices[i].position.y);
			mMax.z = fmax(mMax.z, iVertices[i].position.z);
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