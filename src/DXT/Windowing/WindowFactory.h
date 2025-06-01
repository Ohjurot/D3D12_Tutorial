#pragma once

#include <DXT/Common/OS.h>
#include <DXT/Common/HRException.h>

#include <string>

namespace DXT
{
    /*!
     * @brief Handles window messages
     */
    class IWindowMessageHandler
    {
        public:
            virtual ~IWindowMessageHandler() = default;

            /*!
             * @brief Called when a message is send to the window
             * @param wnd Window handle
             * @param msg Window message id
             * @param wParam WPARAM
             * @param lParam LPARAM
             * @return Handler result code
             */
            virtual LRESULT OnWindowMessage(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam) = 0;
    };

    /*!
     * @brief Responsible for window creating
     */
    class WindowFactory
    {
        public:
            WindowFactory();
            WindowFactory(const WindowFactory&) = delete;
            ~WindowFactory();

            WindowFactory& operator=(const WindowFactory&) = delete;

            /*!
             * @brief Create a native window
             * @param messageHandler Message handler instance
             * @param windowName Name of window
             * @param windowStyle Style of window 
             * @param windowExStyle Extended style of window
             * @param x X Position of the window
             * @param y Y Position of the window
             * @param width Width of the window
             * @param height Height of the window
             * @return Native window HWND handle
             */
            HWND CreateNativeWindow(IWindowMessageHandler& messageHandler, const std::wstring& windowName, DWORD windowStyle, DWORD windowExStyle, int x, int y, int width, int height);

        private:
            /*!
             * @brief Setup the window to be able to forward all message toward the actual window implementation
             * @param wnd Window handle
             * @param msg Window message id
             * @param wParam WPARAM
             * @param lParam LPARAM
             * @return Handler result code
             */
            static LRESULT WndProc_Setup(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam);

            /*!
             * @brief Forwards all window message to the actual implementing window
             * @param wnd Window handle
             * @param msg Window message id
             * @param wParam WPARAM
             * @param lParam LPARAM
             * @return Handler result code
             */
            static LRESULT WndProc_Forward(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam);

        private:
            HINSTANCE m_hinstance;
            ATOM m_classAtom = 0;
    };

    /*!
     * @brief Access to the singleton window factory
     * @return 
     */
    inline WindowFactory& GetWindowFactory()
    {
        static WindowFactory instance;
        return instance;
    }
}
