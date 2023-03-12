#pragma once

#include <string>
#include <sstream>
#include <optional>

namespace Sada {

#define LOG_ERROR Logger::log_error(__FILE__, __LINE__)
#define LOG_WARN Logger::log_warn(__FILE__, __LINE__) 
#define LOG_INFO Logger::log_info(__FILE__, __LINE__)
#define LOG_DEBUG Logger::log_debug(__FILE__, __LINE__)

enum class LogLevel : uint8_t
{
    Error,
    Warning,
    Info,
    Debug
};

class LoggerImpl;

class Logger
{
public:
    enum class Sink : uint8_t
    {
        console,
        file
    };

    static Logger& instance();

    static std::stringstream& log_error(const std::string& filename, uint32_t lineno);
    static std::stringstream& log_warn(const std::string& filename, uint32_t lineno);
    static std::stringstream& log_info(const std::string& filename, uint32_t lineno);
    static std::stringstream& log_debug(const std::string& filename, uint32_t lineno);

    void add_sink(Sink logsink, LogLevel minLogLevel, std::optional<std::string> filename = std::nullopt);

private:
    Logger();
    ~Logger();

    LoggerImpl& m_logger_pImpl;
};

}
