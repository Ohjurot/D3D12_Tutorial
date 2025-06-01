#include "StringConvert.h"

std::string DXT::W2A(std::wstring_view inputString) noexcept
{
    std::string outputString;
    if (!inputString.empty() && inputString.size() <= (size_t)std::numeric_limits<int>::max())
    {
        int requiresSize = WideCharToMultiByte(CP_UTF8, 0, inputString.data(), (int)inputString.size(), nullptr, 0, nullptr, nullptr);
        if (requiresSize > 0)
        {
            outputString.resize(requiresSize);
            if (WideCharToMultiByte(CP_UTF8, 0, inputString.data(), (int)inputString.size(), outputString.data(), (int)outputString.size(), nullptr, nullptr) != requiresSize)
            {
                outputString.clear();
            }
        }
    }
    return outputString;
}

std::wstring DXT::A2W(std::string_view inputString) noexcept
{
    std::wstring outputString;
    if (!inputString.empty() && inputString.size() <= (size_t)std::numeric_limits<int>::max())
    {
        int requiresSize = MultiByteToWideChar(CP_UTF8, 0, inputString.data(), (int)inputString.size(), nullptr, 0);
        if (requiresSize > 0)
        {
            outputString.resize(requiresSize);
            if (MultiByteToWideChar(CP_UTF8, 0, inputString.data(), (int)inputString.size(), outputString.data(), (int)outputString.size())  != requiresSize)
            {
                outputString.clear();
            }
        }
    }
    return outputString;
}
