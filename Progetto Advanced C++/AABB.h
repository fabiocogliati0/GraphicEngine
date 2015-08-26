#pragma once

#include <DirectXMath.h>

namespace GraphicsEngine
{

	//Forward declaration
	struct Vertex;

	class AABB
	{
		
	public:

		AABB();

		AABB(const Vertex* iVertices, int iVerticesNumber);

		const DirectX::XMFLOAT3& getMin() const;

		const DirectX::XMFLOAT3& getMax() const;


	private:

		DirectX::XMFLOAT3 mMin;

		DirectX::XMFLOAT3 mMax;

	};

}