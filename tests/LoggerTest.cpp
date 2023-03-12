#include "Logger.hpp"
#include <thread>
#include <chrono>
#include <array>

using namespace Sada;

int main()
{
    Logger::instance().add_sink(Logger::Sink::console, LogLevel::Debug);

    std::array<uint64_t, 4> count{};
    while(true) {
        LOG_DEBUG << "Count: " << count[0];
        count[0]++;
        LOG_INFO << "Count: " << count[1];
        count[1]++;
        LOG_WARN << "Count: " << count[2];
        count[2]++;
        LOG_ERROR << "Count: " << count[3];
        count[3]++;
        std::this_thread::sleep_for(std::chrono::microseconds(1));
    }
    return 0;
}
