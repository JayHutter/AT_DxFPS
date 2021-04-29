#include "VertexShader.h"

VertexShader::VertexShader(Renderer& renderer, const std::wstring& path)
{
	D3DReadFileToBlob(path.c_str(), &pByteCode);
	GetDevice(renderer)->CreateVertexShader(
		pByteCode->GetBufferPointer(), pByteCode->GetBufferSize(),
		nullptr, &pVertexShader
	);
}

void VertexShader::Bind(Renderer& renderer) noexcept
{
	GetContext(renderer)->VSSetShader(pVertexShader.Get(), nullptr, 0u);
}

ID3DBlob* VertexShader::GetByteCode() const noexcept
{
	return pByteCode.Get();
}
