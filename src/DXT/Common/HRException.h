#pragma once

#include <DXT/Common/OS.h>
#include <DXT/Common/Macro.h>
#include <DXT/Common/Exception.h>

#define DXT_THROWON_HRFAIL(expression, ...) DXT_CODE( \
    HRESULT __hr_code = expression; \
    if(FAILED(__hr_code)) \
    { \
        throw ::DXT::HRException(__hr_code, __VA_ARGS__); \
    } \
)

#define DXT_THROWON_FAIL_GETLASTERROR(expression, ...) DXT_CODE( \
    if(!(expression)) \
    { \
        throw ::DXT::HRException(GetLastError(), __VA_ARGS__); \
    } \
)

namespace DXT
{
    /*!
     * @brief Implements HRESULT lookup for exceptions
     */
    class HRException : public Exception
    {
        public:
            HRException(HRESULT errorCode);
            HRException(HRESULT errorCode, std::string_view what);
            HRException(HRESULT errorCode, std::wstring_view what);

            template<typename... Args>
            HRException(HRESULT errorCode, fmt::format_string<Args...> fstr, Args&&... args)
            {
                CollectTrace();
                SetWhat(errorCode, fmt::format(fstr, std::forward<Args>(args)...));
            }
            template<typename... Args>
            HRException(HRESULT errorCode, fmt::wformat_string<Args...> fstr, Args&&... args)
            {
                CollectTrace();
                SetWhat(errorCode, fmt::format(fstr, std::forward<Args>(args)...));
            }
            
            /*!
             * @brief Retrieve the message for an HRESULT error code
             * @param errorCode Input HRESULT error
             * @return String
             */
            static std::string GetHRESULTMessage(HRESULT errorCode);

        protected:
            void SetWhat(HRESULT errorCode, std::string_view what);
            void SetWhat(HRESULT errorCode, std::wstring_view what);
    };
}
