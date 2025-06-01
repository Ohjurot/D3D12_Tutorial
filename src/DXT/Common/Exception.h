#pragma once

#include <DXT/Common/Macro.h>
#include <DXT/Common/Logging.h>
#include <DXT/Common/StringConvert.h>

#include <fmt/format.h>
#include <fmt/xchar.h>

#include <string>
#include <sstream>
#include <exception>
#include <stacktrace>
#include <string_view>

#define DXT_THROWON_FAIL(expression, ...) DXT_CODE( \
    if(!(expression)) \
    { \
        throw ::DXT::Exception(__VA_ARGS__); \
    } \
)

namespace DXT
{
    /*!
     * @brief Exception base class for us
     */
    class Exception : public std::exception
    {
        public:
            Exception();
            Exception(std::string_view what);
            Exception(std::wstring_view what);

            template<typename... Args>
            Exception(fmt::format_string<Args...> fstr, Args&&... args)
            {
                CollectTrace();
                SetWhat(fmt::format(fstr, std::forward<Args>(args)...));
            }
            template<typename... Args>
            Exception(fmt::wformat_string<Args...> fstr, Args&&... args)
            {
                CollectTrace();
                SetWhat(fmt::format(fstr, std::forward<Args>(args)...));
            }

            /*!
             * @brief Logs the exception to the given loger
             * @param logger Log to log on
             * @param level Log level
             * @param logStacktrace Indicates if stacktrace should be logged aswell
             */
            void LogException(const Logger& logger, LogLevel level, bool logStacktrace = true) const;
            
            inline const std::string& GetWhat() const noexcept
            {
                return m_what;
            }
            inline const std::stacktrace& GetStacktrace() const noexcept
            {
                return m_stacktrace;
            }

            char const* what() const override;

        protected:
            inline void SetWhat(std::string_view what)
            {
                m_what = what;
            }
            inline void SetWhat(std::wstring_view what)
            {
                m_what = W2A(what);
            }
            inline void CollectTrace(size_t skip = 2)
            {
                m_stacktrace = std::stacktrace::current(skip, 99);
            }

        private:
            std::string m_what = "Unknown exception!";
            std::stacktrace m_stacktrace = {};
    };
}
