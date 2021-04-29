#include "Bullet.h"
#include "Bindables.h"
#include "Plane.h"
#include "Enemy.h"

Bullet::Bullet(Renderer& renderer, float x, float y, float z, DirectX::XMFLOAT3 vel) :
	velocity(vel)
{
	namespace dx = DirectX;

	pos = { x, y, z };
	scale = { 0.5f, 0.5f, 0.5f };

	if (!IsStaticInitialized())
	{
		struct Vertex
		{
			dx::XMFLOAT3 pos;
			dx::XMFLOAT3 n;
			dx::XMFLOAT2 tc;
		};
		auto model = Plane::MakeSkinned<Vertex>();
		model.SetNormalsIndependentFlat();

		AddStaticBind(std::make_unique<VertexBuffer>(renderer, model.vertices));
		AddStaticBind(std::make_unique<Texture>(renderer, Surface::FromFile("Textures\\shot.png")));

		AddStaticBind(std::make_unique<Blend>(renderer, true));

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

	if (!billboard) billboard = new Billboard(&rot.y, &rot.x);
	if (!collider) collider = new Collision(scale.x, scale.y, scale.z, pos.x, pos.y, pos.z, false, "Attack");
	lifeTime = 1.5f;
}

void Bullet::Update(float dt) noexcept
{
	pos.x += velocity.x * dt;
	pos.y += velocity.y * dt;
	pos.z += velocity.z * dt;

	collider->UpdatePos(pos.x, pos.y, pos.z);
}

void Bullet::TestCollision(GameObject* other, float dt) noexcept
{
	Collision* col = other->GetCollider();
	if (!col)
	{
		return;
	}

	if (col->Tag() == "Player" || col->Tag() == "Attack")
	{
		return;
	}

	if (collider->Colliding(col))
	{
		if (col->Tag() == "Enemy")
		{
			auto e = dynamic_cast<Enemy*>(other);
			e->Damage(15);
		}

		destroy = true;
	}
}