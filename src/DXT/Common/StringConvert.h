#pragma once

#include <DXT/Common/OS.h>

#include <string>
#include <string_view>

namespace DXT
{
    /*!
     * @brief Converts a wide string to a narrow one
     * @param inputString Input wide string 
     * @return Converted narrow string or empty string in case of error
     */
    std::string W2A(std::wstring_view inputString) noexcept;

    /*!
     * @brief Converts a narrow string to a wide one
     * @param inputString Input narrow string
     * @return Converted wide string or empty string in case of error
     */
    std::wstring A2W(std::string_view inputString) noexcept;
}
