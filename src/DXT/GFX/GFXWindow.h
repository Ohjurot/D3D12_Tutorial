#pragma once

#include <DXT/GFX/GFXSystem.h>
#include <DXT/GFX/GFXInstance.h>
#include <DXT/GFX/GFXCommandList.h>
#include <DXT/Windowing/BasicWindow.h>

#include <array>
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
             * @brief Indicates if the windows size in no longer matching the swap chain size and thus the swap chain required resizing
             * @return True if resize is required
             */
            bool RequiresResizing();

            /*!
             * @brief Resized the swap chain to match the owning windows size
             */
            void ResizeNow();

            /*!
             * @brief Begins a new frame
             * @param cmdList Command list to record on
             * @param clearColorR RED Color of window background
             * @param clearColorG GRENN Color of window background
             * @param clearColorB BLUE Color of window background
             * @param clearColorA ALPHA Color of window background
             */
            void BeginFrame(GFXCommandList& cmdList, float clearColorR = .0f, float clearColorG = .0f, float clearColorB = .0f, float clearColorA = .0f);

            /*!
             * @brief Ends a frame
             * @param cmdList command list to record on
             */
            void EndFrame(GFXCommandList& cmdList);

            /*!
             * @brief Presents the frame to the user
             * @param vsync True indicated v syncing with the monitor
             */
            void Present(bool vsync = true);

            LRESULT OnWindowMessage(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam) override;

        private:
            /*!
             * @brief Retrieves all buffer's from the swap chain and setups view
             */
            void GetSwapChainResources();

            /*!
             * @brief Release all open reference to the swap chain
             */
            void ReleaseSwapChainResources();

        private:
            inline static const auto SWAP_CHAIN_FLAGS = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH | DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;

            Logger m_logger = GetLogger("gfx");

            std::shared_ptr<GFXInstance> m_gfx;
            ComPointer<ID3D12Device> m_device;

            ComPointer<IDXGISwapChain3> m_swapChain;
            uint32_t m_swapChainWidth = 0;
            uint32_t m_swapChainHeight = 0;

            uint32_t m_currentBufferIndex = 0;
            ComPointer<ID3D12DescriptorHeap> m_buffersDescriptorHeap;
            std::array<ComPointer<ID3D12Resource>, BUFFER_COUNT> m_buffers;
            std::array<D3D12_CPU_DESCRIPTOR_HANDLE, BUFFER_COUNT> m_cpuBufferHandle;

    };
}
