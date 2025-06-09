#pragma once

#include <DXT/GFX/GFXWindow.h>
#include <DXT/GFX/GFXCommandList.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_dx12.h>
#include <imgui/imgui_impl_win32.h>

namespace DXT
{
    /*!
     * @brief GFX Window with integrated ImGui
     */
    class UIWindow : public GFXWindow
    {
        public:
            UIWindow(const std::shared_ptr<GFXInstance>& gfxInstance, const std::wstring& name, int x, int y, int width, int height);
            ~UIWindow();

            /*!
             * @brief Stars a new ImGui frame
             */
            void ImGuiNewFrame();

            /*!
             * @brief Draws the current imgui frame
             * @param cmdList Command list used for drawing
             */
            void ImGuiDrawFrame(GFXCommandList& cmdList);

            LRESULT OnWindowMessage(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam) override;

        private:
            ImGuiContext* m_imguiContext = nullptr;

            ComPointer<ID3D12DescriptorHeap> m_imguiDescriptorHeap;
    };
}
