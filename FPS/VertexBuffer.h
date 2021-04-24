#pragma once
#include "Bindable.h"
#include "GraphicThrowMacro.h"

class VertexBuffer : public Bindable
{
public: 
	template<class V>
	VertexBuffer(Renderer& renderer, const std::vector<V>& vertices) : stride(sizeof(V))
	{
		INFOMANAGER(renderer);

		D3D11_BUFFER_DESC bd = {};
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.CPUAccessFlags = 0u;
		bd.MiscFlags = 0u;
		bd.ByteWidth = UINT(sizeof(V) * vertices.size());
		bd.StructureByteStride = sizeof(V);
		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = vertices.data();
		GFX_THROW_INFO(GetDevice(renderer)->CreateBuffer(&bd, &sd, &pVertexBuffer));
	}
	void Bind(Renderer& renderer) noexcept override;
protected:
	UINT stride;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;

};

