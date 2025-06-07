#include <spdlog/spdlog.h>

#include <DXT/Common/OS.h>
#include <DXT/GFX/GFXSystem.h>
#include <DXT/GFX/GFXWindow.h>
#include <DXT/GFX/GFXInstance.h>
#include <DXT/GFX/GFXCommandList.h>

extern "C" { __declspec(dllexport) extern const UINT D3D12SDKVersion = 616; }
extern "C" { __declspec(dllexport) extern const char8_t* D3D12SDKPath = u8".\\"; }

int main()
{
    DXT::InitLogging(DXT::LogLevel::trace);
    try
    {
        auto& gfx = DXT::GetGFXSystem();
        gfx.Init();
        {
            auto gpu = std::make_shared<DXT::GFXInstance>(gfx.GetGpus()[0]);
            auto wnd = std::make_shared<DXT::GFXWindow>(gpu, L"Test Window", 100, 100, 500, 500);

            ComPointer<ID3D12CommandAllocator> cmdAllocator;
            DXT_THROWON_HRFAIL(
                gpu->GetDevice()->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&cmdAllocator)),
                "ID3D12Device::CreateCommandAllocator(...) failed for type D3D12_COMMAND_LIST_TYPE_DIRECT"
            );
            DXT::GFXCommandList cmdList(gpu, cmdAllocator);

            while (!wnd->ShouldClose())
            {
                wnd->HandlePendingWindowMessages();
                if (wnd->RequiresResizing())
                {
                    gpu->FlushQueue(DXT::GFXWindow::BUFFER_COUNT);
                    wnd->ResizeNow();
                }

                wnd->BeginFrame(cmdList, 0.6f, 0.3f, 0.9f, 1.0f);
                wnd->EndFrame(cmdList);
                cmdList.CloseAndExecute();
                gpu->FlushQueue();
                DXT_THROWON_HRFAIL(
                    cmdAllocator->Reset(),
                    "ID3D12CommandAllocator::Reset(...) failed"
                );
                cmdList.Reset(cmdAllocator);

                wnd->Present();

                gfx.PollDebugMessage();
            }
            gpu->FlushQueue(DXT::GFXWindow::BUFFER_COUNT);
        }
        gfx.Shutdown();
    }
    catch (const DXT::Exception& ex)
    {
        ex.LogException(spdlog::default_logger(), DXT::LogLevel::critical);
    }
}
