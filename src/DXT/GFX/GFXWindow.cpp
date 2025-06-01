#include "GFXWindow.h"

DXT::GFXWindow::GFXWindow(const std::shared_ptr<GFXInstance>& gfxInstance, const std::wstring& name, int x, int y, int width, int height) :
    BasicWindow::BasicWindow(name, x, y, width, height),
    m_gfx(gfxInstance)
{
    auto dxgiFactory = GetGFXSystem().GetDXGIFactory();
    auto d3d12CommandQueue = m_gfx->GetDirectCommandQueue();

    DXGI_SWAP_CHAIN_DESC1 swapChainDesc1{};
    swapChainDesc1.Width = GetWidth();
    swapChainDesc1.Height = GetHeight();
    swapChainDesc1.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc1.Stereo = false;
    swapChainDesc1.SampleDesc.Count = 1;
    swapChainDesc1.SampleDesc.Quality = 0;
    swapChainDesc1.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc1.BufferCount = BUFFER_COUNT;
    swapChainDesc1.Scaling = DXGI_SCALING_STRETCH;
    swapChainDesc1.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc1.AlphaMode = DXGI_ALPHA_MODE_IGNORE;
    swapChainDesc1.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH | DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;

    DXGI_SWAP_CHAIN_FULLSCREEN_DESC swapChainFullscreenDesc{};
    swapChainFullscreenDesc.RefreshRate.Numerator = 0;
    swapChainFullscreenDesc.RefreshRate.Denominator = 0;
    swapChainFullscreenDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    swapChainFullscreenDesc.Scaling = DXGI_MODE_SCALING_STRETCHED;
    swapChainFullscreenDesc.Windowed = true;

    DXT_THROWON_HRFAIL(
        dxgiFactory->CreateSwapChainForHwnd(
            d3d12CommandQueue,
            GetNativeHandle(),
            &swapChainDesc1,
            &swapChainFullscreenDesc,
            nullptr,
            &m_swapChain
        ),
        L"IDXGIFactory6::CreateSwapChainForHwnd(...) for window {}", GetName()
    );
}

DXT::GFXWindow::~GFXWindow()
{
    m_swapChain.Release();
}

void DXT::GFXWindow::Present(bool vsync /*= true*/)
{
    HRESULT presentResult = m_swapChain->Present(vsync ? 1 : 0, 0);
    if (presentResult == DXGI_ERROR_DEVICE_REMOVED)
    {
        presentResult = m_gfx->GetDevice()->GetDeviceRemovedReason();
    }
    DXT_THROWON_HRFAIL(presentResult, L"IDXGISwapChain1::Present(...) failed on window {}", GetName());
}

LRESULT DXT::GFXWindow::OnWindowMessage(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    return BasicWindow::OnWindowMessage(wnd, msg, wParam, lParam);
}
