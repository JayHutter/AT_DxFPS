#pragma once
#include "IndexedTriangleList.h"
#include <DirectXMath.h>
#include <initializer_list>

class Plane
{
public:
	template<class V>
	static IndexedTriangleList<V> Make()
	{
		namespace dx = DirectX;

		std::vector<V> vertices(4);
		vertices[0].pos = { -1, 1, 0 };
		vertices[1].pos = { 1, 1, 0 };
		vertices[2].pos = { -1, -1, 0 };
		vertices[3].pos = { 1, -1, 0 };

		return{
			std::move(vertices),{
				0,1,2, 1,3,2
			}
		};
	}

	template<class V>
	static IndexedTriangleList<V> MakeSkinned()
	{
		namespace dx = DirectX;

		constexpr float side = 1.0f / 2.0f;

		std::vector<V> vertices(8);
		vertices[0].pos = { -side, side, 0 };
		vertices[0].tc = { 0, 0};
		vertices[1].pos = { side, side, 0 };
		vertices[1].tc = { 1, 0};
		vertices[2].pos = { -side, -side, 0 };
		vertices[2].tc = { 0, 1};
		vertices[3].pos = { side, -side, 0 };
		vertices[3].tc = { 1, 1};



		return{
			std::move(vertices),{
				0,1,2, 1,3,2
			}
		};
	}

	template<class V>
	static IndexedTriangleList<V> MakeSkinnedBackfacing()
	{
		namespace dx = DirectX;

		constexpr float side = 1.0f / 2.0f;

		std::vector<V> vertices(8);
		vertices[0].pos = { -side, side, 0 };
		vertices[0].tc = { 0, 0 };
		vertices[1].pos = { side, side, 0 };
		vertices[1].tc = { 1, 0 };
		vertices[2].pos = { -side, -side, 0 };
		vertices[2].tc = { 0, 1 };
		vertices[3].pos = { side, -side, 0 };
		vertices[3].tc = { 1, 1 };



		return{
			std::move(vertices),{
				0,1,2, 1,3,2,
				1,0,3, 0,2,3,
			}
		};
	}
};