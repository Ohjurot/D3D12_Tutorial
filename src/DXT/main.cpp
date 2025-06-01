#include <spdlog/spdlog.h>

#include <DXT/Common/OS.h>
#include <DXT/GFX/GFXSystem.h>
#include <DXT/GFX/GFXInstance.h>
#include <DXT/Windowing/BasicWindow.h>

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
            DXT::GFXInstance gpu(gfx.GetGpus()[0]);
            DXT::BasicWindow wnd(L"Test Window", 100, 100, 500, 500);
            while (!wnd.ShouldClose())
            {
                wnd.HandlePendingWindowMessages();

            }
        }
        gfx.Shutdown();
    }
    catch (const DXT::Exception& ex)
    {
        ex.LogException(spdlog::default_logger(), DXT::LogLevel::critical);
    }
}
