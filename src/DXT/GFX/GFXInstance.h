#pragma once

#include <DXT/Common/OS.h>
#include <DXT/Common/Logging.h>

#include <DXT/GFX/GFXSystem.h>

namespace DXT
{
    /*!
     * @brief Handles the "Device" and all core GPU interactions
     */
    class GFXInstance
    {
        public:
            GFXInstance(const GFXGpuDescription& gpu);
            ~GFXInstance();

            /*!
             * @brief Executes the given command lists on the direct queue of the GPU
             * @tparam ...T Types of command lists (need to be castable to ID3D12CommandList*)
             * @param ...lists Lists that should be executed
             */
            template<typename... T>
            void ExecuteCommandLists(T... lists)
            {
                ID3D12CommandList* commandLists[sizeof...(lists)] = {lists...};
                m_directCommandQueue->ExecuteCommandLists(sizeof...(lists), commandLists);
            }

            /*!
             * @brief Signals the direct queue
             * @return Value to check if work has been completed or is still pending
             */
            UINT64 SignalGPU();

            /*!
             * @brief Checks if work has been completed 
             * @param signalingValue Value that was returned by signaling call
             * @return True if signaled work has completed. False otherwise
             */
            bool HasCompleted(UINT64 signalingValue);

            /*!
             * @brief Waits until the work done via the signaling call has completed on the gpu
             * @param signalingValue Value that was returned by signaling call
             */
            void WaitForCompletion(UINT64 signalingValue);

            /*!
             * @brief Flushes the queue for a defined number of times
             * @param flushCounts Number of flushes
             */
            void FlushQueue(size_t flushCounts = 1);

            inline auto GetDevice()
            {
                return m_device;
            }
            inline auto GetDirectCommandQueue()
            {
                return m_directCommandQueue;
            }
            inline auto GetRTVHandleIncement() const
            {
                return m_handleIncrementRTV;
            }

        private:
            Logger m_logger = GetLogger("gfx");

            ComPointer<ID3D12Device> m_device;

            ComPointer<ID3D12CommandQueue> m_directCommandQueue;
            ComPointer<ID3D12Fence> m_directCommandQueueFence;
            UINT64 m_directCommandQueueFenceValue = 0;

            UINT m_handleIncrementRTV = 0;
    };
}
