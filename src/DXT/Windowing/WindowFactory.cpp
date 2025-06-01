#include "WindowFactory.h"

DXT::WindowFactory::WindowFactory()
{
    m_hinstance = GetModuleHandle(nullptr);

    WNDCLASSEXW wcex{};
    wcex.cbSize = sizeof(wcex);
    wcex.style = CS_OWNDC;
    wcex.lpfnWndProc = &WindowFactory::WndProc_Setup;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = m_hinstance;
    wcex.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = nullptr;
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = L"dxt_window";
    wcex.hIconSm = wcex.hIcon;

    m_classAtom = RegisterClassExW(&wcex);
    DXT_THROWON_FAIL_GETLASTERROR(m_classAtom, "RegisterClassExW(...) failed");
}

DXT::WindowFactory::~WindowFactory()
{
    UnregisterClassW((LPCWSTR)m_classAtom, m_hinstance);
}

HWND DXT::WindowFactory::CreateNativeWindow(IWindowMessageHandler& messageHandler, const std::wstring& windowName, DWORD windowStyle, DWORD windowExStyle, int x, int y, int width, int height)
{
    return CreateWindowExW(
        windowExStyle,
        (LPCWSTR)m_classAtom,
        windowName.c_str(),
        windowStyle,
        x,
        y,
        width,
        height,
        nullptr,
        nullptr,
        m_hinstance,
        &messageHandler
    );
}

LRESULT DXT::WindowFactory::WndProc_Setup(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (msg == WM_NCCREATE)
    {
        const auto& createParameter = *(CREATESTRUCTW*)lParam;
        SetWindowLongPtrW(wnd, GWLP_USERDATA, (LONG_PTR)createParameter.lpCreateParams);
        SetWindowLongPtrW(wnd, GWLP_WNDPROC, (LONG_PTR)&WindowFactory::WndProc_Forward);
        return WndProc_Forward(wnd, msg, wParam, lParam);
    }
    return DefWindowProcW(wnd, msg, wParam, lParam);
}

LRESULT DXT::WindowFactory::WndProc_Forward(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    auto* messageHandler = (IWindowMessageHandler*)GetWindowLongPtrW(wnd, GWLP_USERDATA);
    return messageHandler->OnWindowMessage(wnd, msg, wParam, lParam);
}
