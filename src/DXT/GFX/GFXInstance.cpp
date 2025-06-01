#include "GFXInstance.h"

DXT::GFXInstance::GFXInstance(const GFXGpuDescription& gpu)
{
    auto& gfx = GetGFXSystem();
    DXT_THROWON_HRFAIL(
        GetGFXSystem().CreateDevice(gpu, D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&m_device)),
        "D3D12CreateDevice(...) failed for GPU with name {} and feature level D3D_FEATURE_LEVEL_12_0", gpu.GpuName
    );

    D3D12_COMMAND_QUEUE_DESC directCommandQueueDesc{};
    directCommandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    directCommandQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_HIGH;
    directCommandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    directCommandQueueDesc.NodeMask = 0;
    DXT_THROWON_HRFAIL(
        m_device->CreateCommandQueue(&directCommandQueueDesc, IID_PPV_ARGS(&m_directCommandQueue)),
        "ID3D12Device::CreateCommandQueue(...) failed for type D3D12_COMMAND_LIST_TYPE_DIRECT"
    );

    DXT_THROWON_HRFAIL(
        m_device->CreateFence(m_directCommandQueueFenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_directCommandQueueFence)),
        "ID3D12Device::CreateFence(...) failed"
    );
}

DXT::GFXInstance::~GFXInstance()
{
    m_directCommandQueue.Release();
    m_directCommandQueueFence.Release();
    m_device.Release();
}

UINT64 DXT::GFXInstance::SignalGPU()
{
    m_directCommandQueue->Signal(m_directCommandQueueFence, ++m_directCommandQueueFenceValue);
    return m_directCommandQueueFenceValue;
}

bool DXT::GFXInstance::HasCompleted(UINT64 signalingValue)
{
    return m_directCommandQueueFence->GetCompletedValue() >= signalingValue;
}

void DXT::GFXInstance::WaitForCompletion(UINT64 signalingValue)
{
    if (!HasCompleted(signalingValue))
    {
        HANDLE hEvent = CreateEvent(nullptr, false, false, nullptr);
        DXT_THROWON_FAIL_GETLASTERROR(hEvent, "Event creation failed!");
        DXT_THROWON_HRFAIL(
            m_directCommandQueueFence->SetEventOnCompletion(signalingValue, hEvent),
            "ID3D12Fence::SetEventOnCompletion(...) failed for {}", signalingValue
        );
        DXT_THROWON_FAIL(
            WaitForSingleObject(hEvent, 60000) == WAIT_OBJECT_0,
            "WaitForSingleObject(...) failed or ran into timeout"
        );
        CloseHandle(hEvent);
    }
}

void DXT::GFXInstance::FlushQueue(size_t flushCounts)
{
    for (size_t i = 0; i < flushCounts; ++i)
    {
        WaitForCompletion(SignalGPU());
    }
}
