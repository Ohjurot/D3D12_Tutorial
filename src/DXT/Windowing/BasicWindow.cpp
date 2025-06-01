#include "BasicWindow.h"

DXT::BasicWindow::BasicWindow(const std::wstring& name, int x, int y, int width, int height) :
    m_name(name)
{
    static const DWORD STYLE = WS_OVERLAPPEDWINDOW | WS_VISIBLE;
    static const DWORD EX_STYLE = WS_EX_OVERLAPPEDWINDOW | WS_EX_APPWINDOW;

    RECT windowRect{ .left = x, .top = y, .right = x + width, .bottom = y + height };
    DXT_THROWON_FAIL_GETLASTERROR(
        AdjustWindowRectEx(&windowRect, STYLE, false, EX_STYLE),
        "Failed to adjust window rect"
    );

    m_wnd = GetWindowFactory().CreateNativeWindow(
        *this,
        name,
        STYLE,
        EX_STYLE,
        windowRect.left,
        windowRect.top,
        windowRect.right - windowRect.left,
        windowRect.bottom - windowRect.top
    );
    DXT_THROWON_FAIL_GETLASTERROR(m_wnd, "Failed to create window");

    RECT clientRect;
    DXT_THROWON_FAIL_GETLASTERROR(
        GetClientRect(m_wnd, &clientRect), 
        L"GetClientRect(...) failed for window {}", m_name
    );
    m_width = clientRect.right - clientRect.left;
    m_height = clientRect.bottom - clientRect.top;
}

DXT::BasicWindow::~BasicWindow()
{
    if (m_wnd)
    {
        DestroyWindow(m_wnd);
    }
}

void DXT::BasicWindow::HandlePendingWindowMessages()
{
    MSG msg;
    while (PeekMessageW(&msg, m_wnd, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
}

bool DXT::BasicWindow::ShouldClose(bool clearCloseFlag /*= true*/)
{
    bool shouldClose = m_closeReceived;
    if (clearCloseFlag)
    {
        m_closeReceived = false;
    }
    return shouldClose;
}

LRESULT DXT::BasicWindow::OnWindowMessage(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
        case WM_CLOSE:
            m_closeReceived = true;
            spdlog::info(L"Window {} received close request", m_name);
            return 0;
        case WM_SIZE:
            if (wParam == SIZE_MAXIMIZED || wParam == SIZE_RESTORED)
            {
                m_width = LOWORD(lParam);
                m_height = HIWORD(lParam);
            }
            break;
    }
    return DefWindowProcW(wnd, msg, wParam, lParam);
}
