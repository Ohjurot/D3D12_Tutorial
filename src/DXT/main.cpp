#include <spdlog/spdlog.h>

#include <DXT/Common/OS.h>
#include <DXT/Common/Exception.h>
#include <DXT/Common/HRException.h>

extern "C" { __declspec(dllexport) extern const UINT D3D12SDKVersion = 616; }
extern "C" { __declspec(dllexport) extern const char8_t* D3D12SDKPath = u8".\\"; }

int main()
{
    DXT::InitLogging(DXT::LogLevel::trace);
    try
    {
        ComPointer<ID3D12Device> device;
        DXT_THROWON_HRFAIL(
            D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&device)),
            "D3D12CreateDevice(...) failed!"
        );
    }
    catch (const DXT::Exception& ex)
    {
        ex.LogException(spdlog::default_logger(), DXT::LogLevel::critical);
    }

    // We will use spdlog to say hello! 
    // So that you can see that conan works :-)
    spdlog::warn("Hello World from version {}!", DXT_VERSION);
}
