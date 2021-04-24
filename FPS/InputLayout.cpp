#include "InputLayout.h"
#include "GraphicThrowMacro.h"

InputLayout::InputLayout(Renderer& renderer, 
	const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout, 
	ID3DBlob* pVertexShaderByteCode)
{
	INFOMANAGER(renderer);
	
	GFX_THROW_INFO(GetDevice(renderer)->CreateInputLayout(
		layout.data(), (UINT)layout.size(),
		pVertexShaderByteCode->GetBufferPointer(),
		pVertexShaderByteCode->GetBufferSize(),
		&pInputLayout));
}

void InputLayout::Bind(Renderer& renderer) noexcept
{
	GetContext(renderer)->IASetInputLayout(pInputLayout.Get());
}
