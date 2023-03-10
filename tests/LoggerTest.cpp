#include <Logger.hpp>
#include <thread>
#include <chrono>

using namespace Sada;

int main()
{
    Logger::instance().add_sink(Logger::Sink::console);
    Logger::log_debug() << "Inside main";

    uint64_t count = 0;
    while(true) {
        Logger::log_debug() << "Count: " << count;
        ++count;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    return 0;
}
