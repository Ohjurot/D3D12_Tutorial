#include "HRException.h"

DXT::HRException::HRException(HRESULT errorCode)
{
    CollectTrace();
    SetWhat(errorCode, "Unknown Exception");
}

DXT::HRException::HRException(HRESULT errorCode, std::string_view what)
{
    CollectTrace();
    SetWhat(errorCode, what);
}

DXT::HRException::HRException(HRESULT errorCode, std::wstring_view what)
{
    CollectTrace();
    SetWhat(errorCode, what);
}

std::string DXT::HRException::GetHRESULTMessage(HRESULT errorCode)
{
    std::string errorMessage = "Unknown HRESULT";

    char* winErrorMessage = nullptr;
    int winErrorMessageSize = FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_MAX_WIDTH_MASK,
        nullptr,
        errorCode,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL),
        (LPSTR)&winErrorMessage,
        0,
        nullptr
    );
    if (winErrorMessageSize > 0 && winErrorMessage)
    {
        errorMessage = winErrorMessage;
        LocalFree(winErrorMessage);
    }
    return errorMessage;
}

void DXT::HRException::SetWhat(HRESULT errorCode, std::string_view what)
{
    Exception::SetWhat(fmt::format("{} - HRESULT Failed 0x{:08X} {}", what, (uint32_t)errorCode, GetHRESULTMessage(errorCode)));
}

void DXT::HRException::SetWhat(HRESULT errorCode, std::wstring_view what)
{
    SetWhat(errorCode, W2A(what));
}
