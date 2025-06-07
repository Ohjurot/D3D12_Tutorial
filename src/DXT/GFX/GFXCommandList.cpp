#include "GFXCommandList.h"

DXT::GFXCommandList::GFXCommandList(const std::shared_ptr<GFXInstance>& gpu, ID3D12CommandAllocator* allocator) :
    m_gpu(gpu)
{
    DXT_THROWON_HRFAIL(
        gpu->GetDevice()->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, allocator, nullptr, IID_PPV_ARGS(&m_cmdList)),
        "ID3D12Device::CreateCommandList(...) failed for type D3D12_COMMAND_LIST_TYPE_DIRECT"
    );
}

void DXT::GFXCommandList::CloseAndExecute()
{
    FlushBarrieres();
    DXT_THROWON_HRFAIL(
        m_cmdList->Close(),
        "ID3D12GraphicsCommandList::Close(...) failed"
    );
    m_gpu->ExecuteCommandLists(m_cmdList);
}

void DXT::GFXCommandList::Reset(ID3D12CommandAllocator* allocator)
{
    DXT_THROWON_HRFAIL(
        m_cmdList->Reset(allocator, nullptr),
        "ID3D12GraphicsCommandList::Reset(...) failed"
    );
}

DXT::GFXCommandList& DXT::GFXCommandList::ResoureceBarrier(D3D12_RESOURCE_BARRIER&& barrier, bool flushBarriers /*= false*/)
{
    m_pendingBarrieres.push_back(std::move(barrier));
    if (flushBarriers)
    {
        FlushBarrieres();
    }
    return *this;
}

DXT::GFXCommandList& DXT::GFXCommandList::ResoureceBarrier(const D3D12_RESOURCE_BARRIER& barrier, bool flushBarriers /*= false*/)
{
    m_pendingBarrieres.push_back(barrier);
    if (flushBarriers)
    {
        FlushBarrieres();
    }
    return *this;
}

DXT::GFXCommandList& DXT::GFXCommandList::FlushBarrieres()
{
    if (!m_pendingBarrieres.empty())
    {
        m_cmdList->ResourceBarrier(m_pendingBarrieres.size(), m_pendingBarrieres.data());
        m_pendingBarrieres.clear();
    }
    return *this;
}

DXT::GFXCommandList& DXT::GFXCommandList::ResourceTransition(ID3D12Resource* resource, D3D12_RESOURCE_STATES stateBefore, D3D12_RESOURCE_STATES stateAfter, bool flushBarriers /*= false*/)
{
    D3D12_RESOURCE_BARRIER barr;
    barr.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barr.Transition.pResource = resource;
    barr.Transition.Subresource = 0;
    barr.Transition.StateBefore = stateBefore;
    barr.Transition.StateAfter = stateAfter;
    barr.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;

    return ResoureceBarrier(std::move(barr), flushBarriers);
}

DXT::GFXCommandList& DXT::GFXCommandList::ClearRenderTarget(D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle, float r /*= .0f*/, float g /*= .0f*/, float b /*= .0f*/, float a /*= .0f*/)
{
    float clearColor[] = { r, g, b, a };
    m_cmdList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
    return *this;
}

