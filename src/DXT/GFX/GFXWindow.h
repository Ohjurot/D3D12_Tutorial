#pragma once

#include <DXT/GFX/GFXSystem.h>
#include <DXT/GFX/GFXInstance.h>
#include <DXT/Windowing/BasicWindow.h>

#include <memory>

namespace DXT
{
    /*!
     * @brief A SwapChain power D3D12 window
     */
    class GFXWindow : public BasicWindow
    {
        public:
            inline static const UINT BUFFER_COUNT = 2;

        public:
            GFXWindow(const std::shared_ptr<GFXInstance>& gfxInstance, const std::wstring& name, int x, int y, int width, int height);
            ~GFXWindow();

            /*!
             * @brief Presents the frame to the user
             * @param vsync True indicated v syncing with the monitor
             */
            void Present(bool vsync = true);

            LRESULT OnWindowMessage(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam) override;

        private:
            Logger m_logger = GetLogger("gfx");
            std::shared_ptr<GFXInstance> m_gfx;

            ComPointer<IDXGISwapChain1> m_swapChain;
    };
}
