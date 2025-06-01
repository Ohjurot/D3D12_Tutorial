#include "Exception.h"

DXT::Exception::Exception()
{
    CollectTrace();
}

void DXT::Exception::LogException(const Logger& logger, LogLevel level, bool logStacktrace /*= true*/) const
{
    logger->log(level, "Exception occurred:");

    std::stringstream ss;
    ss << GetWhat();

    std::string line;
    while (std::getline(ss, line))
    {
        logger->log(level, "  {}", line);
    }

    if (logStacktrace)
    {
        logger->log(level, "Stacktrace:");

        size_t line = GetStacktrace().size();
        for (const auto& traceElement : GetStacktrace())
        {
            if (traceElement.source_file().empty())
            {
                logger->log(level, "  {:02} - 0x{:016X} {}", line--, (size_t)traceElement.native_handle(), traceElement.description());
            }
            else
            {
                logger->log(level, "  {:02} - 0x{:016X} {} - {}:{}", line--, (size_t)traceElement.native_handle(), traceElement.description(), traceElement.source_file(), traceElement.source_line());
            }
        }
    }
}

DXT::Exception::Exception(std::string_view what)
{
    CollectTrace();
    SetWhat(what);
}

DXT::Exception::Exception(std::wstring_view what)
{
    CollectTrace();
    SetWhat(what);
}

char const* DXT::Exception::what() const
{
    return GetWhat().c_str();    
}
