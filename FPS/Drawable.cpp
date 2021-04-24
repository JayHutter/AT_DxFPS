#include "Drawable.h"
#include "GraphicThrowMacro.h"
#include "IndexBuffer.h"
#include <cassert>
#include <typeinfo>

void Drawable::Draw(Renderer& renderer) const noexcept
{
	for (auto& b : binds)
	{
		b->Bind(renderer);
	}
	for (auto& b : GetStaticBinds())
	{
		b->Bind(renderer);
	}
	renderer.DrawIndexed(pIndexBuffer->GetCount());
}

void Drawable::AddBind(std::unique_ptr<Bindable> bind) noexcept
{
	assert("Use AddIndexBuffer to bind index buffer!" && typeid(*bind) != typeid(IndexBuffer));
	binds.push_back(std::move(bind));
}

void Drawable::AddIndexBuffer(std::unique_ptr<class IndexBuffer> ibuff) noexcept
{
	assert("Already added index buffer" && pIndexBuffer == nullptr);
	pIndexBuffer = ibuff.get();
	binds.push_back(std::move(ibuff));
}

void Drawable::SetCameraDistance(float dist) noexcept
{
	distanceToCam = dist;
}
