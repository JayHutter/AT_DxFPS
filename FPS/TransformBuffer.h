#pragma once
#include "ConstantBuffer.h"
#include "Drawable.h"
#include <DirectXMath.h>

class TransformBuffer : public Bindable
{
public:
	TransformBuffer(Renderer& renderer, const Drawable& parent, UINT slot = 0u);
	void Bind(Renderer& renderer) noexcept override;

private:
	struct Transforms
	{
		DirectX::XMMATRIX modelViewProj;
		DirectX::XMMATRIX model;
	};

	static std::unique_ptr<VertexConstantBuffer<Transforms>> pVCBuff;
	const Drawable& parent;
};

