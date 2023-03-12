cmake_minimum_required(VERSION 3.16)

project(LoggerTest)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

message(${PROJECT_BINARY_DIR})
link_directories(${PROJECT_BINARY_DIR}/../)

enable_testing()

add_executable(LoggerTest LoggerTest.cpp)

target_link_libraries(LoggerTest GTest::gtest_main -lSadaLogger)

include(GoogleTest)
gtest_discover_tests(LoggerTest)

add_test(LoggerGTest LoggerTest)
