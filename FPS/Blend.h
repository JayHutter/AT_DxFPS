#pragma once
#include "Bindable.h"
#include <array>

class Blend : public Bindable
{
public:
	Blend(Renderer& renderer, bool blending);
	void Bind(Renderer& renderer) noexcept override;
protected:
	Microsoft::WRL::ComPtr<ID3D11BlendState> pBlend;
	bool blending;
};

