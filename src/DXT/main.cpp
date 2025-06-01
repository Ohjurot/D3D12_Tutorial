#include <spdlog/spdlog.h>

#include <DXT/Common/OS.h>
#include <DXT/GFX/GFXSystem.h>
#include <DXT/GFX/GFXWindow.h>
#include <DXT/GFX/GFXInstance.h>

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
            while (!wnd->ShouldClose())
            {
                wnd->HandlePendingWindowMessages();
                if (wnd->RequiresResizing())
                {
                    gpu->FlushQueue(DXT::GFXWindow::BUFFER_COUNT);
                    wnd->ResizeNow();
                }

                // TODO: Work on the gpu
                gpu->FlushQueue();
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
