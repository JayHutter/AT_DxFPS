#include "Wall.h"
#include "Bindables.h"
#include "Cube.h"
#include "Surface.h"
#include "Texture.h"
#include "Sampler.h"

Wall::Wall(Renderer& renderer, int x, int y, int flr)
{
	float gridScale = renderer.GridScale();

	pos = { x * gridScale, flr * gridScale * 2.0f, y * gridScale };
	scale = { gridScale, gridScale * 2.0f, gridScale };

	namespace dx = DirectX;

	if (!IsStaticInitialized())
	{
		struct Vertex
		{
			dx::XMFLOAT3 pos;
			dx::XMFLOAT3 n;
			dx::XMFLOAT2 tc;
		};
		auto model = Cube::MakeIndependentTextured<Vertex>();
		model.SetNormalsIndependentFlat();

		for (auto& v : model.vertices)
		{
			v.tc.y *= 2;
		}
		
		AddStaticBind(std::make_unique<VertexBuffer>(renderer, model.vertices));
		AddStaticBind(std::make_unique<Texture>(renderer, Surface::FromFile("Textures\\bricks.png")));

		AddStaticBind(std::make_unique<Sampler>(renderer));

		auto pvs = std::make_unique<VertexShader>(renderer, L"TexturedLitVS.cso");
		auto pvsbc = pvs->GetByteCode();
		AddStaticBind(std::move(pvs));

		AddStaticBind(std::make_unique<PixelShader>(renderer, L"TexturedLitPS.cso"));

		AddStaticIndexBuffer(std::make_unique<IndexBuffer>(renderer, model.indices));

		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
			{ "Normal",0,DXGI_FORMAT_R32G32B32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 },
			{ "TexCoord",0,DXGI_FORMAT_R32G32_FLOAT,0,24,D3D11_INPUT_PER_VERTEX_DATA,0 },
		};
		AddStaticBind(std::make_unique<InputLayout>(renderer, ied, pvsbc));

		AddStaticBind(std::make_unique<Topology>(renderer, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

		struct PSMaterialConstant
		{
			float specularIntensity = 0.6f;
			float specularPower = 30.0f;
			float padding[2];
		} colorConst;
		AddStaticBind(std::make_unique<PixelConstantBuffer<PSMaterialConstant>>(renderer, colorConst, 1u));
	}
	else
	{
		SetIndexFromStatic();
	}

	AddBind(std::make_unique<TransformBuffer>(renderer, *this));

	// model deformation transform (per instance, not stored as bind)
	//dx::XMStoreFloat3x3(
	//	&mt,
	//	dx::XMMatrixScaling(20.0f, 100.0f, 100.0f)
	//);

	blockAi = true;
	if (!collider) collider = new Collision(scale.x, scale.y, scale.z, pos.x, pos.y, pos.z, false, "World");
}

void Wall::Update(float dt) noexcept
{
}

