#include "Box.h"
#include "Bindables.h"
#include "Cube.h"

Box::Box(Renderer& renderer, float x, float y, float z)
{
	namespace dx = DirectX;

	pos = { x, y, z };

	dyaw = 2;
	dpitch = 2;
	droll = 2;

	if (!IsStaticInitialized())
	{
		struct Vertex
		{
			dx::XMFLOAT3 pos;
			dx::XMFLOAT3 n;
		};
		auto model = Cube::MakeIndependent<Vertex>();
		model.SetNormalsIndependentFlat();

		AddStaticBind(std::make_unique<VertexBuffer>(renderer, model.vertices));

		auto pvs = std::make_unique<VertexShader>(renderer, L"PhongLitVS.cso");
		auto pvsbc = pvs->GetByteCode();
		AddStaticBind(std::move(pvs));

		AddStaticBind(std::make_unique<PixelShader>(renderer, L"PhongLitPS.cso"));

		AddStaticIndexBuffer(std::make_unique<IndexBuffer>(renderer, model.indices));

		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
			{ "Normal",0,DXGI_FORMAT_R32G32B32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 },
		};
		AddStaticBind(std::make_unique<InputLayout>(renderer, ied, pvsbc));

		AddStaticBind(std::make_unique<Topology>(renderer, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	}
	else
	{
		SetIndexFromStatic();
	}

	AddBind(std::make_unique<TransformBuffer>(renderer, *this));

	struct PSMaterialConstant
	{
		alignas(16) dx::XMFLOAT3 color;
		float specularIntensity = 0.6f;
		float specularPower = 30.0f;
		float padding[2];
	} colorConst;
	colorConst.color = DirectX::XMFLOAT3(0.7f, 0.7f, 0.9f);
	AddBind(std::make_unique<PixelConstantBuffer<PSMaterialConstant>>(renderer, colorConst, 1u));

	// model deformation transform (per instance, not stored as bind)
	//dx::XMStoreFloat3x3(
	//	&mt,
	//	dx::XMMatrixScaling(1.0f, 1.0f, 1.0f)
	//);
}

void Box::Update(float dt) noexcept
{
	rot.x += dyaw * dt;
	rot.y += dpitch * dt;
	rot.z += droll * dt;
}