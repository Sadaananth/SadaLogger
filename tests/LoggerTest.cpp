#include <Logger.hpp>
#include <thread>
#include <chrono>

using namespace Sada;

int main()
{
    Logger::instance().add_sink(Logger::Sink::console);
    //LOG_DEBUG << "Inside main";
    LOG_DEBUG << "This is a debug message";
    LOG_INFO << "This is a info message";
    LOG_WARN << "This is a warning message";
    LOG_ERROR << "This is a error message";

    // uint64_t count = 0;
    while(true) {
        //LOG_DEBUG << "Count: " << count;
        //++count;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    return 0;
}
