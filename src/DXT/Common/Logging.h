#pragma once

#include <spdlog/spdlog.h>

#include <mutex>
#include <memory>

namespace DXT
{
    /*!
     * @brief Spdlog logger alias
     */
    using Logger = std::shared_ptr<spdlog::logger>;

    /*!
     * @brief Spdlog log level alias
     */
    using LogLevel = spdlog::level::level_enum;

    /*!
     * @brief Initializes the logging subsystem
     * @param minimumLogLevel Minimum level that should be logged
     */
    void InitLogging(LogLevel minimumLogLevel);

    /*!
     * @brief Retrieves a logger with a given name
     * @param loggerName Name of logger
     * @return Logger pointer
     */
    Logger GetLogger(const std::string& loggerName);
}
