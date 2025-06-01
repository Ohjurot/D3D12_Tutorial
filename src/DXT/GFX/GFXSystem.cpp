#include "GFXSystem.h"

void DXT::GFXSystem::Init()
{
    #ifdef DXT_DEBUG
    DXT_THROWON_HRFAIL(
        DXGIGetDebugInterface1(0, IID_PPV_ARGS(&m_dxgiDebug)),
        "DXGIGetDebugInterface1(...) failed for IDXGIDebug"
    );
    DXT_THROWON_HRFAIL(
        DXGIGetDebugInterface1(0, IID_PPV_ARGS(&m_dxgiInfoQueue)),
        "DXGIGetDebugInterface1(...) failed for IDXGIInfoQueue"
    );
    DXT_THROWON_HRFAIL(
        D3D12GetDebugInterface(IID_PPV_ARGS(&m_d3d12Debug)),
        "D3D12GetDebugInterface(...) failed for ID3D12Debug5"
    );

    m_d3d12Debug->EnableDebugLayer();
    m_d3d12Debug->SetEnableAutoName(true);
    #endif

    UINT dxgiFactoryCreateFlag = 0;
    #ifdef DXT_DEBUG
    dxgiFactoryCreateFlag = DXGI_CREATE_FACTORY_DEBUG;
    #endif
    DXT_THROWON_HRFAIL(
        CreateDXGIFactory2(dxgiFactoryCreateFlag, IID_PPV_ARGS(&m_dxgiFactory)),
        "CreateDXGIFactory2(...) failed for IDXGIFactory7"
    );

    ComPointer<IDXGIAdapter> gpuAdapter;
    for (
        UINT gpuIndex = 0;
        m_dxgiFactory->EnumAdapterByGpuPreference(gpuIndex, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&gpuAdapter)) == S_OK;
        ++gpuIndex
        )
    {
        DXGI_ADAPTER_DESC adapterDescription;
        DXT_THROWON_HRFAIL(
            gpuAdapter->GetDesc(&adapterDescription),
            "IDXGIAdapter::GetDesc(...) failed"
        );
        m_gpus.push_back({
            .GpuName = W2A(adapterDescription.Description),
            .MemorySize = adapterDescription.DedicatedVideoMemory,
            .GpuId = adapterDescription.AdapterLuid
        });
        m_logger->info(L"Detected GPU ({}) {}", gpuIndex, adapterDescription.Description);
        gpuAdapter.Release();
    }
}

void DXT::GFXSystem::Shutdown()
{
    m_dxgiFactory.Release();

    #ifdef DXT_DEBUG
    m_d3d12Debug.Release();
    
    DXT_THROWON_HRFAIL(
        m_dxgiDebug->ReportLiveObjects(DXGI_DEBUG_ALL, (DXGI_DEBUG_RLO_FLAGS)(DXGI_DEBUG_RLO_DETAIL | DXGI_DEBUG_RLO_IGNORE_INTERNAL)),
        "IDXGIDebug::ReportLiveObjects(...) failed"
    );
    PollDebugMessage();

    m_dxgiInfoQueue.Release();
    m_dxgiDebug.Release();
    #endif
}

void DXT::GFXSystem::PollDebugMessage()
{
    #ifdef DXT_DEBUG
    for (UINT64 i = 0; i < m_dxgiInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL); ++i)
    {
        size_t messageSize = 0;
        m_dxgiInfoQueue->GetMessageW(DXGI_DEBUG_ALL, i, nullptr, &messageSize);
        DXT_THROWON_FAIL(messageSize > 0, "IDXGIInfoQueue::GetMessageW(...) failed for DXGI_DEBUG_ALL at index {} (peek)", i);
        if (m_dxgiInfoQueueBuffer.size() < messageSize)
        {
            m_dxgiInfoQueueBuffer.resize(messageSize);
        }

        auto& message = *(DXGI_INFO_QUEUE_MESSAGE*)m_dxgiInfoQueueBuffer.data();
        DXT_THROWON_HRFAIL(
            m_dxgiInfoQueue->GetMessageW(DXGI_DEBUG_ALL, i, &message, &messageSize),
            "IDXGIInfoQueue::GetMessageW(...) failed for DXGI_DEBUG_ALL at index {} (retrival)", i
        );

        LogLevel messageLogLevel = LogLevel::info;
        const char* messageProducer = "UNKNOWN";
        const char* messageCathegory = "UNKNOWN";
        const char* messageSeverity = "INFO";

        if (memcmp(&message.Producer, &DXGI_DEBUG_D3D12, sizeof(message.Producer)) == 0)
        {
            messageProducer = "D3D12";
        }
        else if (memcmp(&message.Producer, &DXGI_DEBUG_DXGI, sizeof(message.Producer)) == 0)
        {
            messageProducer = "DXGI";
        }
        else if (memcmp(&message.Producer, &DXGI_DEBUG_APP, sizeof(message.Producer)) == 0)
        {
            messageProducer = "APP";
        }

        switch (message.Category)
        {
            case DXGI_INFO_QUEUE_MESSAGE_CATEGORY_UNKNOWN:
                messageCathegory = "UNKNOWN";
                break;
            case DXGI_INFO_QUEUE_MESSAGE_CATEGORY_MISCELLANEOUS:
                messageCathegory = "MISCELLANEOUS";
                break;
            case DXGI_INFO_QUEUE_MESSAGE_CATEGORY_INITIALIZATION:
                messageCathegory = "INITIALIZATION";
                break;
            case DXGI_INFO_QUEUE_MESSAGE_CATEGORY_CLEANUP:
                messageCathegory = "CLEANUP";
                break;
            case DXGI_INFO_QUEUE_MESSAGE_CATEGORY_COMPILATION:
                messageCathegory = "COMPILATION";
                break;
            case DXGI_INFO_QUEUE_MESSAGE_CATEGORY_STATE_CREATION:
                messageCathegory = "STATE_CREATION";
                break;
            case DXGI_INFO_QUEUE_MESSAGE_CATEGORY_STATE_SETTING:
                messageCathegory = "STATE_SETTING";
                break;
            case DXGI_INFO_QUEUE_MESSAGE_CATEGORY_STATE_GETTING:
                messageCathegory = "STATE_GETTING";
                break;
            case DXGI_INFO_QUEUE_MESSAGE_CATEGORY_RESOURCE_MANIPULATION:
                messageCathegory = "RESOURCE_MANIPULATION";
                break;
            case DXGI_INFO_QUEUE_MESSAGE_CATEGORY_EXECUTION:
                messageCathegory = "EXECUTION";
                break;
            case DXGI_INFO_QUEUE_MESSAGE_CATEGORY_SHADER:
                messageCathegory = "SHADER";
                break;
        }

        switch (message.Severity)
        {
            case DXGI_INFO_QUEUE_MESSAGE_SEVERITY_CORRUPTION:
                messageSeverity = "CORRUPTION";
                messageLogLevel = LogLevel::critical;
                break;
            case DXGI_INFO_QUEUE_MESSAGE_SEVERITY_ERROR:
                messageSeverity = "ERROR";
                messageLogLevel = LogLevel::err;
                break;
            case DXGI_INFO_QUEUE_MESSAGE_SEVERITY_WARNING:
                messageSeverity = "WARNING";
                messageLogLevel = LogLevel::warn;
                break;
            case DXGI_INFO_QUEUE_MESSAGE_SEVERITY_INFO:
                messageSeverity = "INFO";
                messageLogLevel = LogLevel::info;
                break;
            case DXGI_INFO_QUEUE_MESSAGE_SEVERITY_MESSAGE:
                messageSeverity = "MESSAGE";
                messageLogLevel = LogLevel::debug;
                break;
        }

        m_logger->log(messageLogLevel, "{} {} ({}): {}", messageProducer, messageSeverity, messageCathegory, message.pDescription);
    }
    m_dxgiInfoQueue->ClearStoredMessages(DXGI_DEBUG_ALL);
    #endif
}

HRESULT DXT::GFXSystem::CreateDevice(const GFXGpuDescription& gpu, D3D_FEATURE_LEVEL minimumDeviceLevel, REFIID riid, void** ppDevice)
{
    ComPointer<IDXGIAdapter> adapter;
    DXT_THROWON_HRFAIL(
        m_dxgiFactory->EnumAdapterByLuid(gpu.GpuId, IID_PPV_ARGS(&adapter)),
        "IDXGIFactory6::EnumAdapterByLuid(...) for GPU with name {}", gpu.GpuName
    );
    return D3D12CreateDevice(adapter, minimumDeviceLevel, riid, ppDevice);
}
