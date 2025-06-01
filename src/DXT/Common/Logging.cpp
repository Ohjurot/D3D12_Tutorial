#include "Logging.h"

void DXT::InitLogging(LogLevel minimumLogLevel)
{
    auto defaultLogger = spdlog::default_logger()->clone("dxt");
    defaultLogger->set_level(minimumLogLevel);
    defaultLogger->set_pattern("[%d.%m.%Y %H:%M:%S.%e] [%^%l%$] [%t] [%n] %v");
    spdlog::set_default_logger(defaultLogger);
}

DXT::Logger DXT::GetLogger(const std::string& loggerName)
{
    static std::mutex s_lock;
    std::lock_guard janitor(s_lock);

    Logger logger = spdlog::get(loggerName);
    if (!logger)
    {
        logger = spdlog::default_logger()->clone(loggerName);
        spdlog::register_logger(logger);
    }
    return logger;
}
