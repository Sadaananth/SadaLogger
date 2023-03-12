#include "Logger.hpp"
#include <thread>
#include <queue>
#include <iostream>
#include <sstream>
#include <mutex>
#include <fstream>
#include <memory>
#include <atomic>
#include <chrono>

namespace Sada {

std::string to_string(LogLevel loglevel)
{
    switch(loglevel) {
        case LogLevel::Error:
            return "Error";
        case LogLevel::Warning:
            return "Warning";
        case LogLevel::Info:
            return "Info";
        case LogLevel::Debug:
            return "Debug";
    }

    throw std::runtime_error("Invalid log level");
}

struct LogLine
{
    LogLine() {}
    LogLine(LogLevel level, const std::string& filename, const std::string& lineno);
    LogLine(const LogLine&& logLine);

    void operator = (const LogLine& log_line);

    LogLevel m_log_level;
    std::string m_file_name;
    std::string m_line_no;
    std::stringstream m_line_stream;
};

LogLine::LogLine(LogLevel level, const std::string& filename, const std::string& lineno)
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
    LogSink(LogLevel log_level);
    virtual void print(const std::string& data) = 0;

private:
    LogLevel m_min_log_level;
};

LogSink::LogSink(LogLevel log_level)
    : m_min_log_level(log_level)
{
    
}

class FileSink : public LogSink
{
public:
    FileSink(const std::string& file_path, LogLevel log_level);

    void print(const std::string& data) override;

private:
    std::ofstream m_out_stream;
};

FileSink::FileSink(const std::string& file_path, LogLevel log_level)
    : LogSink(log_level)
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
    ConsoleSink(LogLevel log_level);
    void print(const std::string& data) override;
};

ConsoleSink::ConsoleSink(LogLevel log_level)
    : LogSink(log_level)
{

}

void ConsoleSink::print(const std::string& data)
{
    std::cout << data << std::endl;
}

class LoggerImpl
{
public:
    static LoggerImpl& instance();

    std::stringstream& get_log_stream(LogLevel loglevel, const std::string& filename, const std::string& lineno);

    void add_sink(std::unique_ptr<LogSink> log_sink);
private:
    LoggerImpl();
    ~LoggerImpl();

    void run();
    void sink_output(const LogLine& log_line);
    std::string format_output(const LogLine& log_line);

    std::thread m_thread;
    std::atomic_bool m_run_thread{false};

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
    m_run_thread = true;
    m_thread = std::thread(&LoggerImpl::run, this);
}

LoggerImpl::~LoggerImpl()
{
    if(m_run_thread) {
        //std::cout << "Stopping thread" << std::endl;
        m_run_thread = false;
        m_thread.join();
    }
}

std::stringstream& LoggerImpl::get_log_stream(LogLevel loglevel, const std::string& filename, const std::string& lineno)
{
    LogLine log_line(loglevel, filename, lineno);

    //std::cout << to_string(loglevel) << " Queue size:" << m_log_list.size() << std::endl;
    std::lock_guard<std::mutex> lock(m_mutex);
    m_log_list.push(std::move(log_line));
    auto& stream = m_log_list.back().m_line_stream;

    return stream;
}

void LoggerImpl::add_sink(std::unique_ptr<LogSink> log_sink)
{
    m_sink_list.push_back(std::move(log_sink));
}

void LoggerImpl::run()
{
    while(m_run_thread || !m_log_list.empty()) {
        std::this_thread::sleep_for(std::chrono::microseconds(10));
        if(m_log_list.empty()) {
            continue;
        }
        
        //std::cout << "Taking log message" << std::endl;
        LogLine log_line;
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            log_line = m_log_list.front();
            if(log_line.m_line_stream.str().empty()) {
                continue;
            }
            m_log_list.pop();
        }
        //std::cout << "Message taken" << std::endl;
        sink_output(log_line);
    }
    //std::cout << "Thread stopped" << std::endl;
}

void LoggerImpl::sink_output(const LogLine& log_line)
{
    for(auto& sink : m_sink_list) {
        auto output = format_output(log_line);
        //std::cout << "Output to be printed " << output << std::endl;
        sink->print(output);
    }
}

std::string LoggerImpl::format_output(const LogLine& log_line)
{
    std::stringstream outputstream;

    outputstream << "[" << to_string(log_line.m_log_level) << "] " << log_line.m_file_name << ":" << log_line.m_line_no
        << " " << log_line.m_line_stream.str() << std::endl;
    
    return outputstream.str();
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

void Logger::add_sink(Logger::Sink logsink, LogLevel minLogLevel, std::optional<std::string> filename)
{
    switch(logsink) {
        case Sink::console:
        {
            m_logger_pImpl.add_sink(std::make_unique<ConsoleSink>(ConsoleSink(minLogLevel)));
            break;
        }
        case Sink::file:
        {   
            if(filename.has_value()) {
                m_logger_pImpl.add_sink(std::make_unique<FileSink>(FileSink(filename.value(), minLogLevel)));
            }
            break;
        }
    }
}

std::stringstream& Logger::log_error(const std::string& filename, uint32_t lineno)
{
    return Logger::instance().m_logger_pImpl.get_log_stream(LogLevel::Error, filename, std::to_string(lineno));
}

std::stringstream& Logger::log_warn(const std::string& filename, uint32_t lineno)
{
    return Logger::instance().m_logger_pImpl.get_log_stream(LogLevel::Warning, filename, std::to_string(lineno));
}

std::stringstream& Logger::log_info(const std::string& filename, uint32_t lineno)
{
    return Logger::instance().m_logger_pImpl.get_log_stream(LogLevel::Info, filename, std::to_string(lineno));
}

std::stringstream& Logger::log_debug(const std::string& filename, uint32_t lineno)
{
    return Logger::instance().m_logger_pImpl.get_log_stream(LogLevel::Debug, filename, std::to_string(lineno));
}

}
