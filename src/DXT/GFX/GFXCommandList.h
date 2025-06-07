#pragma once

#include <DXT/GFX/GFXInstance.h>

#include <memory>
#include <vector>

namespace DXT
{
    /*!
     * @brief 
     */
    class GFXCommandList
    {
        public:
            GFXCommandList(const std::shared_ptr<GFXInstance>& gpu, ID3D12CommandAllocator* allocator);
            GFXCommandList(const GFXCommandList&) = delete;
            GFXCommandList(GFXCommandList&&) noexcept = default;

            GFXCommandList& operator=(const GFXCommandList&) = delete;
            GFXCommandList& operator=(GFXCommandList&&) noexcept = default;

            /*!
             * @brief Closes the command list and executes it on the gpu instance
             */
            void CloseAndExecute();

            /*!
             * @brief Resets the command list to the given allocator
             * @param allocator 
             */
            void Reset(ID3D12CommandAllocator* allocator);

            /*!
             * @brief Queue a barrier for flushing
             * @param barrier Movable barrier
             * @param flushBarriers If true also flushes/commits the barrier
             */
            GFXCommandList& ResoureceBarrier(D3D12_RESOURCE_BARRIER&& barrier, bool flushBarriers = false);

            /*!
             * @brief Queue a barrier for flushing
             * @param barrier Copyable barrier
             * @param flushBarriers If true also flushes/commits the barrier
             */
            GFXCommandList& ResoureceBarrier(const D3D12_RESOURCE_BARRIER& barrier, bool flushBarriers = false);

            /*!
             * @brief Places all pending barriers to the cmd list
             */
            GFXCommandList& FlushBarrieres();

            /*!
             * @brief Introduces a resource state transition (as barrier)
             * @param resource Resource to transited
             * @param stateBefore State before
             * @param stateAfter State after
             * @param flushBarriers True if barriers shall be flushed
             * @return Self-ref for fluent interface
             */
            GFXCommandList& ResourceTransition(ID3D12Resource* resource, D3D12_RESOURCE_STATES stateBefore, D3D12_RESOURCE_STATES stateAfter, bool flushBarriers = false);

            /*!
             * @brief Clears the render target
             * @param rtvHandle Descriptor to specify rtv to clear
             * @param r RED Component of the clear color
             * @param g GREEN Component of the clear color
             * @param b BLUE Component of the clear color
             * @param a ALPHA Component of the clear color
             * @return Self-ref for fluent interface
             */
            GFXCommandList& ClearRenderTarget(D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle, float r = .0f, float g = .0f, float b = .0f, float a = .0f);

            // Expose command list

            inline auto Get()
            {
                return m_cmdList;
            }
            inline operator auto* ()
            {
                return m_cmdList.Get();
            }

        private:
            std::shared_ptr<GFXInstance> m_gpu;
            ComPointer<ID3D12GraphicsCommandList> m_cmdList;

            std::vector<D3D12_RESOURCE_BARRIER> m_pendingBarrieres;
    };
}
