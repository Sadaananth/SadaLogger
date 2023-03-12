#include "Logger.hpp"
#include <gtest/gtest.h>

using namespace Sada;

std::stringstream& get_output(const std::string& message) {
  //std::stringstream& stream = LOG_DEBUG << message.c_str();
  return LOG_DEBUG;
}

TEST(LoggerTest, noThrowTest) {
  EXPECT_NO_THROW(Logger::instance());
  EXPECT_NO_THROW(LOG_DEBUG << "Debug Message");
  EXPECT_NO_THROW(LOG_WARN << "Warning Message");
  EXPECT_NO_THROW(LOG_INFO << "Info Message");
  EXPECT_NO_THROW(LOG_ERROR << "Error Message");
}

TEST(LoggerTest, checkDebugLog) {
  std::string message = "Debug Message";
  std::stringstream stream;
  stream << message;
  EXPECT_EQ(get_output(message).str(), stream.str());

  message = "Info message";
  stream.clear();
  stream << message;
  EXPECT_EQ(get_output(message).str(), stream.str());

  message = "Warning message";
  stream.clear();
  stream << message;
  EXPECT_EQ(get_output(message).str(), stream.str());

  message = "Error message";
  stream.clear();
  stream << message;
  EXPECT_EQ(get_output(message).str(), stream.str());
}
