#include <spdlog/spdlog.h>

#include <d3d12.h>
#include <d3dx12/d3dx12.h>

extern "C" { __declspec(dllexport) extern const UINT D3D12SDKVersion = 616; }
extern "C" { __declspec(dllexport) extern const char8_t* D3D12SDKPath = u8".\\"; }

int main()
{
    ID3D12Device1* dev;
    D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&dev));

    // We will use spdlog to say hello! 
    // So that you can see that conan works :-)
    spdlog::warn("Hello World from version {}!", DXT_VERSION);
}
