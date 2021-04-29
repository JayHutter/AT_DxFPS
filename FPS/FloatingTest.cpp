#include "FloatingTest.h"
#include "Bindables.h"
#include "Cube.h"

FloatingTest::FloatingTest(Renderer& renderer, float x, float y, float z, float width, float height, float depth)
	: x(x), y(y), z(z), width(width), height(height), depth(depth)
{
	roll = 225.0f;
	pitch = 45.0f;
	yaw = 45.0f;

	namespace dx = DirectX;

	if (!IsStaticInitialized())
	{
		struct Vertex
		{
			dx::XMFLOAT3 pos;
		};
		const auto model = Cube::Make<Vertex>();
		AddStaticBind(std::make_unique<VertexBuffer>(renderer, model.vertices));

		auto pvs = std::make_unique<VertexShader>(renderer, L"VertexShader.cso");
		auto pvsbc = pvs->GetByteCode();
		AddStaticBind(std::move(pvs));

		AddStaticBind(std::make_unique<PixelShader>(renderer, L"PixelShader.cso"));

		AddStaticIndexBuffer(std::make_unique<IndexBuffer>(renderer, model.indices));

		struct PixelShaderConstants
		{
			struct
			{
				float r;
				float g;
				float b;
				float a;
			} face_colors[8];
		};
		const PixelShaderConstants psc =
		{
			{
				{ 1.0f,1.0f,1.0f },
				{ 1.0f,0.0f,0.0f },
				{ 0.0f,1.0f,0.0f },
				{ 1.0f,1.0f,0.0f },
				{ 0.0f,0.0f,1.0f },
				{ 1.0f,0.0f,1.0f },
				{ 0.0f,1.0f,1.0f },
				{ 0.0f,0.0f,0.0f },
			}
		};

		AddStaticBind(std::make_unique<PixelConstantBuffer<PixelShaderConstants>>(renderer, psc));

		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		};
		AddStaticBind(std::make_unique<InputLayout>(renderer, ied, pvsbc));

		AddStaticBind(std::make_unique<Topology>(renderer, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	}
	else
	{
		SetIndexFromStatic();
	}

	AddBind(std::make_unique<TransformBuffer>(renderer, *this));

	// model deformation transform (per instance, not stored as bind)
	dx::XMStoreFloat3x3(
		&mt,
		dx::XMMatrixScaling(width, height, depth)
	);
}

void FloatingTest::Update(float dt) noexcept
{
	timer += dt;
	roll += droll * dt;
	pitch += dpitch * dt;
	yaw += dyaw * dt;
	theta += dtheta * dt;
	phi += dphi * dt;
	chi += dchi * dt;

	y += speed * dt;

	if (timer > 0.5f)
	{
		speed *= -1;
		timer = 0;
	}
}

DirectX::XMMATRIX FloatingTest::GetTransformXM() const noexcept
{
	return DirectX::XMMatrixScaling(width, height, depth) *
		DirectX::XMMatrixTranslation(x, y, z) *
		DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll);
	//DirectX::XMMatrixTranslation(0.0f, 0.0f, 20.0f);
}
