#include "Bindable.h"

ID3D11DeviceContext* Bindable::GetContext(Renderer& renderer) noexcept
{
    return renderer.pContext.Get();
}

ID3D11Device* Bindable::GetDevice(Renderer& renderer) noexcept
{
    return renderer.pDevice.Get();
}
