#include "PixelShader.h"
#include "GraphicThrowMacro.h"

PixelShader::PixelShader(Renderer& renderer, const std::wstring& path)
{
	INFOMANAGER(renderer);

	Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
	GFX_THROW_INFO(D3DReadFileToBlob(path.c_str(), &pBlob));
	GFX_THROW_INFO(GetDevice(renderer)->CreatePixelShader(pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(), nullptr, &pPixelShader));
}

void PixelShader::Bind(Renderer& renderer) noexcept
{
	GetContext(renderer)->PSSetShader(pPixelShader.Get(), nullptr, 0u);
}
