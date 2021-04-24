#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <vector>
#include <wrl.h>
#include <d3dcompiler.h>
#include "Exception.h"
#include <vector>
#include "DxgiInfoManager.h"
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <memory>
#include <random>
#include "Camera.h"

namespace wrl = Microsoft::WRL;

class Renderer
{
	friend class Bindable;
public:
	class GraphicException : public Exception
	{
		using Exception::Exception;
	};

	class HrException : public GraphicException
	{
	public:
		HrException(int line, const char* file, HRESULT hr, std::vector<std::string> infoMsgs = {}) noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorString() const noexcept;
		std::string GetErrorDescription() const noexcept;
		std::string GetErrorInfo() const noexcept;
	private:
		HRESULT hr;
		std::string info;
	};
	class InfoException : public GraphicException
	{
	public:
		InfoException(int line, const char* file, std::vector<std::string> infoMsgs) noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		std::string GetErrorInfo() const noexcept;
	private:
		std::string info;
	};
	class DeviceException : public HrException
	{
		using HrException::HrException;
	public:
		const char* GetType() const noexcept override;
	};

	Renderer(HWND hWnd);
	~Renderer() = default;
	Renderer(const Renderer&) = delete;
	Renderer& operator= (const Renderer&) = delete;
	 
	void DrawFrame();
	void ClearBuffer(float red, float green, float blue) noexcept;

	void TestDraw(float x, float y, float z, float angle);
	void DrawIndexed(UINT count) noexcept;
	void SetProjection(DirectX::FXMMATRIX proj) noexcept;
	DirectX::XMMATRIX GetProjection() const noexcept;
	Camera* GetCamera() noexcept;

	float GridScale() const noexcept;
private:
	DirectX::XMMATRIX projection;
	Camera* cam;
	const float gridScale = 5.0f;
#ifndef NDEBUG
	DxgiInfoManager infoManager;
#endif

	wrl::ComPtr<ID3D11Device> pDevice;
	wrl::ComPtr<IDXGISwapChain> pSwapChain;
	wrl::ComPtr<ID3D11DeviceContext> pContext;
	wrl::ComPtr<ID3D11RenderTargetView> pRenderTarget;
	wrl::ComPtr<ID3D11DepthStencilView> pDepthView;
};

