#include "Blend.h"
#include "GraphicThrowMacro.h"

Blend::Blend(Renderer& renderer, bool blending) : blending(blending)
{
	INFOMANAGER(renderer);

	D3D11_BLEND_DESC blendDesc = {};
	auto& brt = blendDesc.RenderTarget[0];
	if (blending)
	{
		brt.BlendEnable = true;
		brt.SrcBlend = D3D11_BLEND_SRC_ALPHA;
		brt.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		brt.BlendOp = D3D11_BLEND_OP_ADD;
		brt.SrcBlendAlpha = D3D11_BLEND_ZERO;
		brt.DestBlendAlpha = D3D11_BLEND_ZERO;
		brt.BlendOpAlpha = D3D11_BLEND_OP_ADD;
		brt.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	}
	else
	{
		brt.BlendEnable = false;
		brt.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	}

	GFX_THROW_INFO(GetDevice(renderer)->CreateBlendState(&blendDesc, &pBlend));
}

void Blend::Bind(Renderer& renderer) noexcept
{
	GetContext(renderer)->OMSetBlendState(pBlend.Get(), nullptr, 0xFFFFFFFFu);
}
