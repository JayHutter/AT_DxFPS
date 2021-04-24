#include "PointLight.h"

PointLight::PointLight(Renderer& renderer, float x, float y, float f, float intensity)
	:
	cbuf(renderer)
{
	float gridScale = renderer.GridScale();

	this->intensity = intensity;

	Reset();
	SetPos(x * gridScale, f + gridScale * 0.5f, y * gridScale);
}

void PointLight::Reset() noexcept
{
	cbData = {
		{ 0.0f,0.0f,0.0f },
		{ 0.15f,0.15f,0.15f },
		{ 1.0f,1.0f,1.0f },
		intensity,
		1.0f,
		0.045f,
		0.0075f ,
	};
}

void PointLight::Draw(Renderer& renderer) const noexcept
{
}

void PointLight::Bind(Renderer& renderer, DirectX::FXMMATRIX view) const noexcept
{
	auto dataCopy = cbData;
	const auto pos = DirectX::XMLoadFloat3(&cbData.pos);
	DirectX::XMStoreFloat3(&dataCopy.pos, DirectX::XMVector3Transform(pos, view));
	cbuf.Update(renderer, dataCopy);
	cbuf.Bind(renderer);
}

void PointLight::SetPos(float x, float y, float z)
{
	cbData.pos.x = x;
	cbData.pos.y = y;
	cbData.pos.z = z;
}

void PointLight::Move(float x, float y, float z)
{
	cbData.pos.x += x;
	cbData.pos.y += y;
	cbData.pos.z += z;
}
