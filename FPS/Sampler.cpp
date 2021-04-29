#include "Sampler.h"

Sampler::Sampler(Renderer& renderer)
{
	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	GetDevice(renderer)->CreateSamplerState(&samplerDesc, &pSampler);
}

void Sampler::Bind(Renderer& renderer) noexcept
{
	GetContext(renderer)->PSSetSamplers(0, 1, pSampler.GetAddressOf());
}