#pragma once

#include <DXT/Common/OS.h>
#include <DXT/Common/Logging.h>
#include <DXT/Common/HRException.h>

#include <string>
#include <vector>

namespace DXT
{
    /*!
     * @brief Describes a GPU
     */
    struct GFXGpuDescription
    {
        /*!
         * @brief Name of the gpu
         */
        std::string GpuName;

        /*!
         * @brief VRAM size of the GPU
         */
        size_t MemorySize;

        /*!
         * @brief Local ID of the GPU
         */
        LUID GpuId;
    };

    /*!
     * @brief Singleton manager of D3D12 and DXGI
     */
    class GFXSystem
    {
        public:
            GFXSystem() = default;
            GFXSystem(const GFXSystem&) = delete;
            GFXSystem& operator=(const GFXSystem&) = delete;

            /*!
             * @brief Initializes the GFX subsystem
             */
            void Init();

            /*!
             * @brief Un-Initializes the GFX subsystem
             */
            void Shutdown();

            /*!
             * @brief Polls debug message when debugging (for D3D12 and DXGI)
             */
            void PollDebugMessage();

            /*!
             * @brief Retrieve all detected GPUs
             * @return Vector of GPUs 
             */
            inline const auto& GetGpus() const
            {
                return m_gpus;
            }

            /*!
             * @brief Exposes the factory for user otherwher
             * @return ComPointer to the swapchain
             */
            inline auto GetDXGIFactory()
            {
                return m_dxgiFactory;
            }

            /*!
             * @brief 
             * @param gpu GPU (adapter) To create device for
             * @param minimumDeviceLevel Minimum device feature level
             * @param riid IID Of interface
             * @param ppDevice Pointer to Pointer of interface
             * @return HRESULT error code
             */
            HRESULT CreateDevice(const GFXGpuDescription& gpu, D3D_FEATURE_LEVEL minimumDeviceLevel, REFIID riid, void** ppDevice);

        private:
            Logger m_logger = GetLogger("gfx");

            ComPointer<IDXGIFactory6> m_dxgiFactory;

            std::vector<GFXGpuDescription> m_gpus;

            #ifdef DXT_DEBUG
            ComPointer<IDXGIDebug> m_dxgiDebug;
            ComPointer<ID3D12Debug5> m_d3d12Debug;

            ComPointer<IDXGIInfoQueue> m_dxgiInfoQueue;
            std::vector<char> m_dxgiInfoQueueBuffer;
            #endif
    };

    /*!
     * @brief 
     * @return Reference to gfx system
     */
    inline GFXSystem& GetGFXSystem()
    {
        static GFXSystem instance;
        return instance;
    }
}
