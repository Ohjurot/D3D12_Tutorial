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
    inline static GFXSystem& GetGFXSystem()
    {
        static GFXSystem instance;
        return instance;
    }
}
