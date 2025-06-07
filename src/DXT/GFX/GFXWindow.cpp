#include "GFXWindow.h"

DXT::GFXWindow::GFXWindow(const std::shared_ptr<GFXInstance>& gfxInstance, const std::wstring& name, int x, int y, int width, int height) :
    BasicWindow::BasicWindow(name, x, y, width, height),
    m_gfx(gfxInstance),
    m_device(gfxInstance->GetDevice())
{
    auto dxgiFactory = GetGFXSystem().GetDXGIFactory();
    auto d3d12CommandQueue = m_gfx->GetDirectCommandQueue();

    D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDescription{};
    descriptorHeapDescription.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    descriptorHeapDescription.NumDescriptors = BUFFER_COUNT;
    descriptorHeapDescription.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    descriptorHeapDescription.NodeMask = 0;
    DXT_THROWON_HRFAIL(
        m_device->CreateDescriptorHeap(&descriptorHeapDescription, IID_PPV_ARGS(&m_buffersDescriptorHeap)),
        "ID3D12Device::CreateDescriptorHeap(...) failed for type D3D12_DESCRIPTOR_HEAP_TYPE_RTV with size {}", BUFFER_COUNT
    );

    auto firstCPUHandle = m_buffersDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
    for (size_t i = 0; i < BUFFER_COUNT; ++i)
    {
        m_cpuBufferHandle[i].ptr = firstCPUHandle.ptr + (i * m_gfx->GetRTVHandleIncement());
    }

    DXGI_SWAP_CHAIN_DESC1 swapChainDesc1{};
    swapChainDesc1.Width = m_swapChainWidth = GetWidth();
    swapChainDesc1.Height = m_swapChainHeight = GetHeight();
    swapChainDesc1.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc1.Stereo = false;
    swapChainDesc1.SampleDesc.Count = 1;
    swapChainDesc1.SampleDesc.Quality = 0;
    swapChainDesc1.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc1.BufferCount = BUFFER_COUNT;
    swapChainDesc1.Scaling = DXGI_SCALING_STRETCH;
    swapChainDesc1.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc1.AlphaMode = DXGI_ALPHA_MODE_IGNORE;
    swapChainDesc1.Flags = SWAP_CHAIN_FLAGS;

    DXGI_SWAP_CHAIN_FULLSCREEN_DESC swapChainFullscreenDesc{};
    swapChainFullscreenDesc.RefreshRate.Numerator = 0;
    swapChainFullscreenDesc.RefreshRate.Denominator = 0;
    swapChainFullscreenDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    swapChainFullscreenDesc.Scaling = DXGI_MODE_SCALING_STRETCHED;
    swapChainFullscreenDesc.Windowed = true;

    m_logger->info(L"Initalizing swapchain to {}x{} for window \"{}\"", m_swapChainWidth, m_swapChainHeight, GetName());
    ComPointer<IDXGISwapChain1> tempSwapChain;
    DXT_THROWON_HRFAIL(
        dxgiFactory->CreateSwapChainForHwnd(
            d3d12CommandQueue,
            GetNativeHandle(),
            &swapChainDesc1,
            &swapChainFullscreenDesc,
            nullptr,
            &tempSwapChain
        ),
        L"IDXGIFactory6::CreateSwapChainForHwnd(...) for window \"{}\" in {}x{}", GetName(), m_swapChainWidth, m_swapChainHeight
    );
    DXT_THROWON_FAIL(
        tempSwapChain.QueryInterface(m_swapChain),
        "Can't case IDXGISwapChain1 to IDXGISwapChain3"
    );

    GetSwapChainResources();
}

DXT::GFXWindow::~GFXWindow()
{
    ReleaseSwapChainResources();
    m_device.Release();
    m_swapChain.Release();
    m_buffersDescriptorHeap.Release();
}

bool DXT::GFXWindow::RequiresResizing()
{
    return m_swapChainWidth != GetWidth() || m_swapChainHeight != GetHeight();
}

void DXT::GFXWindow::ResizeNow()
{
    ReleaseSwapChainResources();

    m_swapChainWidth = GetWidth();
    m_swapChainHeight = GetHeight();
    m_logger->info(L"Resizing swapchain to {}x{} for window \"{}\"", m_swapChainWidth, m_swapChainHeight, GetName());
    DXT_THROWON_HRFAIL(
        m_swapChain->ResizeBuffers(0, m_swapChainWidth, m_swapChainHeight, DXGI_FORMAT_UNKNOWN, SWAP_CHAIN_FLAGS),
        L"IDXGISwapChain1::ResizeBuffers(...) failed for window \"{}\" in {}x{}", GetName(), m_swapChainWidth, m_swapChainHeight
    );

    GetSwapChainResources();
}

void DXT::GFXWindow::BeginFrame(GFXCommandList& cmdList, float clearColorR, float clearColorG, float clearColorB, float clearColorA)
{
    m_currentBufferIndex = m_swapChain->GetCurrentBackBufferIndex();

    cmdList.ResourceTransition(
        m_buffers[m_currentBufferIndex],
        D3D12_RESOURCE_STATE_PRESENT,
        D3D12_RESOURCE_STATE_RENDER_TARGET
    );
    cmdList.FlushBarrieres();
    cmdList.ClearRenderTarget(m_cpuBufferHandle[m_currentBufferIndex], clearColorR, clearColorG, clearColorB, clearColorA);
}

void DXT::GFXWindow::EndFrame(GFXCommandList& cmdList)
{
    cmdList.ResourceTransition(
        m_buffers[m_currentBufferIndex],
        D3D12_RESOURCE_STATE_RENDER_TARGET,
        D3D12_RESOURCE_STATE_PRESENT
    );
    cmdList.FlushBarrieres();
}

void DXT::GFXWindow::Present(bool vsync /*= true*/)
{
    HRESULT presentResult = m_swapChain->Present(vsync ? 1 : 0, 0);
    if (presentResult == DXGI_ERROR_DEVICE_REMOVED)
    {
        presentResult = m_device->GetDeviceRemovedReason();
    }
    DXT_THROWON_HRFAIL(presentResult, L"IDXGISwapChain1::Present(...) failed on window \"{}\"", GetName());
}

LRESULT DXT::GFXWindow::OnWindowMessage(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    return BasicWindow::OnWindowMessage(wnd, msg, wParam, lParam);
}

void DXT::GFXWindow::GetSwapChainResources()
{
    for (size_t i = 0; i < BUFFER_COUNT; ++i)
    {
        DXT_THROWON_HRFAIL(
            m_swapChain->GetBuffer(i, IID_PPV_ARGS(&m_buffers[i])),
            L"IDXGISwapChain1::GetBuffer(...) failed at index {} for window \"{}\"", i , GetName()
        );
        m_device->CreateRenderTargetView(m_buffers[i], nullptr, m_cpuBufferHandle[i]);
    }
}

void DXT::GFXWindow::ReleaseSwapChainResources()
{
    for (size_t i = 0; i < BUFFER_COUNT; ++i)
    {
        m_buffers[i].Release();
    }
}
