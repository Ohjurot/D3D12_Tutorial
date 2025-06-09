#include "UIWindow.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

DXT::UIWindow::UIWindow(const std::shared_ptr<GFXInstance>& gfxInstance, const std::wstring& name, int x, int y, int width, int height) :
    GFXWindow(gfxInstance, name, x, y, width, height)
{
    D3D12_DESCRIPTOR_HEAP_DESC imguiDescritorHeapDesc;
    imguiDescritorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    imguiDescritorHeapDesc.NumDescriptors = 1;
    imguiDescritorHeapDesc.NodeMask = 0;
    imguiDescritorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

    DXT_THROWON_HRFAIL(
        gfxInstance->GetDevice()->CreateDescriptorHeap(&imguiDescritorHeapDesc, IID_PPV_ARGS(&m_imguiDescriptorHeap)),
        "ID3D12Device::CreateDescriptorHeap(...) failed"
    );

    m_imguiContext = ImGui::CreateContext();

    DXT_THROWON_FAIL(
        ImGui_ImplWin32_Init(GetNativeHandle()),
        "ImGui_ImplWin32_Init(...) failed"
    );

    DXT_THROWON_FAIL(
        ImGui_ImplDX12_Init(
            gfxInstance->GetDevice(),
            BUFFER_COUNT,
            DXGI_FORMAT_R8G8B8A8_UNORM,
            m_imguiDescriptorHeap,
            m_imguiDescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
            m_imguiDescriptorHeap->GetGPUDescriptorHandleForHeapStart()
        ),
        "ImGui_ImplDX12_Init(...) failed"
    );
}

DXT::UIWindow::~UIWindow()
{
    ImGui_ImplDX12_Shutdown();
    ImGui_ImplWin32_Shutdown();
}

void DXT::UIWindow::ImGuiNewFrame()
{
    ImGui_ImplDX12_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
}

void DXT::UIWindow::ImGuiDrawFrame(GFXCommandList& cmdList)
{
    cmdList.Get()->SetDescriptorHeaps(1, &m_imguiDescriptorHeap);

    ImGui::Render();
    ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), cmdList);
}

LRESULT DXT::UIWindow::OnWindowMessage(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(wnd, msg, wParam, lParam))
    {
        return true;
    }
    return GFXWindow::OnWindowMessage(wnd, msg, wParam, lParam);
}
