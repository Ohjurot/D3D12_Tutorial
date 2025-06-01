#pragma once

#include <DXT/Common/HRException.h>
#include <DXT/Windowing/WindowFactory.h>

namespace DXT
{
    /*!
     * @brief Implements a basic windows window
     */
    class BasicWindow : public IWindowMessageHandler
    {
        public:
            BasicWindow(const std::wstring& name, int x, int y, int width, int height);
            ~BasicWindow();

            /*!
             * @brief Handles all message that where pending on the window
             */
            void HandlePendingWindowMessages();

            /*!
             * @brief Checking if the window should close
             * @param clearCloseFlag If true clear the closing flag
             * @return True if the window should close
             */
            bool ShouldClose(bool clearCloseFlag = true);

            inline const std::wstring& GetName() const
            {
                return m_name;
            }
            inline auto GetWidth() const
            {
                return m_width;
            }
            inline auto GetHeight() const
            {
                return m_height;
            }

            LRESULT OnWindowMessage(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam) override;

        protected:
            /*!
             * @brief Exposes the windows native window handle
             * @return HWND handle associated with this window
             */
            inline HWND GetNativeHandle()
            {
                return m_wnd;
            }

        private:
            HWND m_wnd;
            std::wstring m_name;
            bool m_closeReceived = false;

            uint32_t m_width = 0;
            uint32_t m_height = 0;
    };
}
