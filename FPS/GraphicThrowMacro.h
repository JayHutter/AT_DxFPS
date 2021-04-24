#pragma once
#define GFX_EXCEPT_NOINFO(hr) Renderer::HrException(__LINE, __FILE, (hr))
#define GFX_THROW_NOINFO (hrcall) if(FAILED(hr = (hrcall))) throw Renderer::HrException(__LINE__, __FILE__, hr)

#ifndef NDEBUG
#define GFX_EXCEPT(hr) Renderer::HrException(__LINE__, __FILE__, (hr), infoManager.GetMessages())
#define GFX_THROW_INFO(hrcall) if (FAILED(hr = (hrcall))) throw GFX_EXCEPT(hr)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Renderer::DeviceException(__LINE__, __FILE__, (hr), infoManager.GetMessages())
#define GFX_THROW_INFO_ONLY(call) infoManager.Set(); (call); {auto v = infoManager.GetMessages(); if(!v.empty()) {throw Renderer::InfoException(__LINE__, __FILE__, v);}}
#else
#define GFX_EXCEPT(hr) Renderer::HrException(__LINE__, __FILE, (hr))
#define GFX_THROW_INFO(hrcall) GFX_THROW_NOINFO(hrcall)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Renderer::DeviceException(__LINE__, __FILE__, (hr))
#define GFX_THROW_INFO_ONLY(call) (call)
#endif

#ifdef NDEBUG
#define INFOMANAGER(render) HRESULT hr
#else
#define INFOMANAGER(render) HRESULT hr; DxgiInfoManager& infoManager = GetInfoManager((render))
#endif