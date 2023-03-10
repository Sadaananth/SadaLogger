#pragma once

#include <string>
#include <sstream>
#include <optional>

namespace Sada {

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

    static std::stringstream& log_error();
    static std::stringstream& log_warn();
    static std::stringstream& log_info();
    static std::stringstream& log_debug();

    void add_sink(Sink logsink, std::optional<std::string> filename = std::nullopt);
private:
    Logger();
    ~Logger();

    LoggerImpl& m_logger_pImpl;
};

}
