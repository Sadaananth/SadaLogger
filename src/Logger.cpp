#include "Logger.hpp"
#include <thread>
#include <queue>
#include <iostream>
#include <sstream>
#include <mutex>
#include <fstream>
#include <memory>

enum class LogLevel : uint8_t
{
    Error,
    Warning,
    Info,
    Debug
};

struct LogLine
{
    LogLine() {}
    LogLine(LogLevel level, const std::string& filename, uint32_t lineno);
    LogLine(const LogLine&& logLine);

    void operator = (const LogLine& log_line);

    LogLevel m_log_level;
    std::string m_file_name;
    uint32_t m_line_no;
    std::stringstream m_line_stream;
};

LogLine::LogLine(LogLevel level, const std::string& filename, uint32_t lineno)
    : m_log_level(level)
    , m_file_name(filename)
    , m_line_no(lineno)
{

}

LogLine::LogLine(const LogLine&& log_line)
{
    m_log_level = log_line.m_log_level;
    m_file_name = log_line.m_file_name;
    m_line_no = log_line.m_line_no;
    m_line_stream << log_line.m_line_stream.str();
}

void LogLine::operator = (const LogLine& log_line)
{
    m_log_level = log_line.m_log_level;
    m_file_name = log_line.m_file_name;
    m_line_no = log_line.m_line_no;
    m_line_stream << log_line.m_line_stream.str();
}

class LogSink
{
public:
    virtual void print(const std::string& data) = 0;
};

class FileSink : public LogSink
{
public:
    FileSink(const std::string& file_path);

    void print(const std::string& data) override;

private:
    std::ofstream m_out_stream;
};

FileSink::FileSink(const std::string& file_path)
{
    m_out_stream.open(file_path);
    if(!m_out_stream.is_open()) {
        throw std::runtime_error("Failed to open file " + file_path);
    }
}

void FileSink::print(const std::string& data)
{
    m_out_stream << data << std::endl;
}

class ConsoleSink : public LogSink
{
public:
    void print(const std::string& data) override;
};

void ConsoleSink::print(const std::string& data)
{
    std::cout << data << std::endl;
}

class LoggerImpl
{
public:
    static LoggerImpl& instance();

    std::stringstream& get_log_stream(LogLevel loglevel, const std::string& filename, uint32_t lineno);

    void add_sink(std::unique_ptr<LogSink> log_sink);
private:
    LoggerImpl();
    ~LoggerImpl() {}

    void log(LogLevel level);
    void run();
    void sink_output(const LogLine& log_line);

    std::thread m_thread;

    std::queue<LogLine> m_log_list;
    std::mutex m_mutex;

    std::vector<std::unique_ptr<LogSink>> m_sink_list;
};

LoggerImpl& LoggerImpl::instance()
{
    static LoggerImpl loggerImpl;
    return loggerImpl;
}

LoggerImpl::LoggerImpl()
{
    m_thread = std::thread(&LoggerImpl::run, this);
}

std::stringstream& LoggerImpl::get_log_stream(LogLevel loglevel, const std::string& filename, uint32_t lineno)
{
    LogLine log_line(loglevel, filename, lineno);

    std::lock_guard<std::mutex> lock(m_mutex);
    m_log_list.push(std::move(log_line));
    auto& stream = m_log_list.front().m_line_stream;

    return stream;
}

void LoggerImpl::add_sink(std::unique_ptr<LogSink> log_sink)
{
    m_sink_list.push_back(std::move(log_sink));
}

void LoggerImpl::log(LogLevel level)
{
    std::stringstream stream;

    
}

void LoggerImpl::run()
{
    while(true) {
        if(!m_log_list.empty()) {
            
            LogLine log_line;
            {
                std::lock_guard<std::mutex> lock(m_mutex);
                log_line = m_log_list.front();
                if(log_line.m_line_stream.str().empty()) {
                    continue;
                }
                m_log_list.pop();
            }
            sink_output(log_line);
        }
    }
}

void LoggerImpl::sink_output(const LogLine& log_line)
{
    for(auto& sink : m_sink_list) {
        sink->print(log_line.m_line_stream.str());
    }
}

Logger& Logger::instance()
{
    static Logger logger;
    return logger;
}

Logger::Logger() : m_logger_pImpl(LoggerImpl::instance())
{

}

Logger::~Logger()
{

}

void Logger::add_sink(Logger::Sink logsink, std::optional<std::string> filename)
{
    switch(logsink) {
        case Sink::console:
        {
            m_logger_pImpl.add_sink(std::make_unique<ConsoleSink>(ConsoleSink()));
            break;
        }
        case Sink::file:
        {   
            if(filename.has_value()) {
                m_logger_pImpl.add_sink(std::make_unique<FileSink>(FileSink(filename.value())));
            }
            break;
        }
    }
}

std::stringstream& Logger::log_error()
{
    return Logger::instance().m_logger_pImpl.get_log_stream(LogLevel::Error, "", 20);
}

std::stringstream& Logger::log_warn()
{
    return Logger::instance().m_logger_pImpl.get_log_stream(LogLevel::Warning, "", 20);
}

std::stringstream& Logger::log_info()
{
    return Logger::instance().m_logger_pImpl.get_log_stream(LogLevel::Info, "", 20);
}

std::stringstream& Logger::log_debug()
{
    return Logger::instance().m_logger_pImpl.get_log_stream(LogLevel::Debug, "", 20);
}
