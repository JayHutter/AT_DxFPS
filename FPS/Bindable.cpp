#include "Bindable.h"

ID3D11DeviceContext* Bindable::GetContext(Renderer& renderer) noexcept
{
    return renderer.pContext.Get();
}

ID3D11Device* Bindable::GetDevice(Renderer& renderer) noexcept
{
    return renderer.pDevice.Get();
}

DxgiInfoManager& Bindable::GetInfoManager(Renderer& renderer) noexcept
{
#ifndef NDEBUG
    return renderer.infoManager;
#else
    throw std::logic_error("Attempted to access info manager in release!");
#endif
}
