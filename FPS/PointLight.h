#pragma once
#include "Renderer.h"
#include "ConstantBuffer.h"

class PointLight
{
public:
	PointLight(Renderer& renderer, float x, float y, float f, float intensity);
	void Reset() noexcept;
	void Draw(Renderer& renderer) const noexcept;
	void Bind(Renderer& renderer, DirectX::FXMMATRIX view) const noexcept;
	void SetPos(float x, float y, float z);
	void Move(float x, float y, float z);
private:
	struct PointLightCBuf
	{
		alignas(16) DirectX::XMFLOAT3 pos;
		alignas(16) DirectX::XMFLOAT3 ambient;
		alignas(16) DirectX::XMFLOAT3 diffuse;
		float diffuseIntensity;
		float attConst;
		float attLin;
		float attQuad;
	};
private:
	PointLightCBuf cbData;
	mutable PixelConstantBuffer<PointLightCBuf> cbuf;

	float intensity = 10;
};
