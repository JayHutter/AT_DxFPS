#include "TransformBuffer.h"

TransformBuffer::TransformBuffer(Renderer& renderer, const Drawable& parent, UINT slot)
	:
	parent(parent)
{
	if (!pVCBuff)
	{
		pVCBuff = std::make_unique<VertexConstantBuffer<Transforms>>(renderer, slot);
	}
}

void TransformBuffer::Bind(Renderer& renderer) noexcept
{
	const auto modelView = parent.GetTransformXM() * renderer.GetCamera()->GetTransform();
	const Transforms trans =
	{
		DirectX::XMMatrixTranspose(modelView),
		DirectX::XMMatrixTranspose(
			modelView *
			renderer.GetProjection())
	};
	pVCBuff->Update(renderer, trans);
	pVCBuff->Bind(renderer);
}

std::unique_ptr<VertexConstantBuffer<TransformBuffer::Transforms>> TransformBuffer::pVCBuff;

//renderer.GetCamera()->GetTransform() * 