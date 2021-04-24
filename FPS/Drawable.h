#pragma once
#include "Renderer.h"
#include <DirectXMath.h>
#include <memory>

#include "Collision.h"
#include "Billboard.h"

#include "GameObject.h"

class Bindable;

class Drawable : public GameObject
{
	template<class T>
	friend class DrawableBase;
public:
	Drawable() = default;
	Drawable(const Drawable&) = delete;

	void Draw(Renderer& renderer) const noexcept override;
	void AddBind(std::unique_ptr<Bindable> bind) noexcept;
	void AddIndexBuffer(std::unique_ptr<class IndexBuffer> ibuff) noexcept;

	virtual void SetCameraDistance(float dist) noexcept override;

	virtual ~Drawable() = default;
private:
	virtual const std::vector<std::unique_ptr<Bindable>>& GetStaticBinds() const noexcept = 0;

	const class IndexBuffer* pIndexBuffer = nullptr;
	std::vector<std::unique_ptr<Bindable>> binds;
};

