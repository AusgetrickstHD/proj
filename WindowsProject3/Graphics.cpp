#include "Graphics.h"
#include <dxgi.h>
#include "DxErr.h"
#include <iostream>
#include <sstream>

#pragma comment(lib, "d3d11.lib")

#define GFX_THROW_FAILED(hrcall) if(FAILED(hr = (hrcall))) throw Graphics::HrException(__LINE__,__FILE__,hr);
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemovedException(__LINE__,__FILE__,(hr));

Graphics::Graphics(HWND hWnd)
{
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 0;
    sd.BufferDesc.RefreshRate.Denominator = 0;
    sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.BufferCount = 1;
    sd.OutputWindow = (HWND)696969;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    sd.Flags = 0;

    HRESULT hr;

    GFX_THROW_FAILED(D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        D3D11_CREATE_DEVICE_DEBUG,
        nullptr,
        0,
        D3D11_SDK_VERSION,
        &sd,
        &pSwapChain,
        &pDevice,
        nullptr,
        &pContext
    ));

    ID3D11Resource* pBackBuffer = nullptr;
    GFX_THROW_FAILED(pSwapChain->GetBuffer(0, 
        __uuidof(ID3D11Resource), 
        reinterpret_cast<void**>(&pBackBuffer)));
    GFX_THROW_FAILED(pDevice->CreateRenderTargetView(
        pBackBuffer,
        nullptr,
        &pTarget
    ));
    pBackBuffer->Release();
}

Graphics::~Graphics()
{
	if (pDevice != nullptr) {
		pDevice->Release();
	}
	if (pContext != nullptr) {
		pContext->Release();
	}
	if (pTarget != nullptr) {
		pTarget->Release();
	}
	if (pSwapChain != nullptr) {
		pSwapChain->Release();
	}
}


void Graphics::EndFrame() {
    HRESULT hr;
    if (FAILED(hr = pSwapChain->Present(1u, 0u))) {
        if (hr == DXGI_ERROR_DEVICE_REMOVED) {
            throw GFX_DEVICE_REMOVED_EXCEPT(pDevice->GetDeviceRemovedReason());
        }
        else {
            GFX_THROW_FAILED(hr);
        }
    }
}


void Graphics::ClearBuffer(float red, float green, float blue) noexcept {
    const float color[] = { red,green,blue, 1.0f };
    pContext->ClearRenderTargetView(pTarget, color);
}

Graphics::HrException::HrException(int line, const char* file, HRESULT hr) noexcept
    :
    Exception(line, file),
    hr(hr)
{}

const char* Graphics::HrException::what() const noexcept {
    std::ostringstream oss;
    oss << GetType() << std::endl
        << "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode()
        << std::dec << " (" << (unsigned long)GetErrorCode() << ")" << std::endl
        << "[Error String] " << GetErrorString() << std::endl
        << "[Description] " << GetErrorDescription() << std::endl
        << GetOriginString();
    whatBuffer = oss.str();
    return whatBuffer.c_str();
}

const char* Graphics::HrException::GetType() const noexcept {
    return "Graphics Exception";
}

HRESULT Graphics::HrException::GetErrorCode() const noexcept {
    return hr;
}

std::string Graphics::HrException::GetErrorString() const noexcept {
    return DXGetErrorStringA(hr);
}

std::string Graphics::HrException::GetErrorDescription() const noexcept {
    char buf[512];
    DXGetErrorDescriptionA(hr, buf, sizeof(buf));
    return buf;
}


const char* Graphics::DeviceRemovedException::GetType() const noexcept {
    return "Graphics Exception [Device Removed] (DXGI_ERROR_DEVICE_REMOVED)";
}
