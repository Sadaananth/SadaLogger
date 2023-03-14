cmake_minimum_required(VERSION 3.16)

project(LoggerGTest)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

link_directories(${PROJECT_BINARY_DIR}/../)

enable_testing()

add_executable(LoggerGTest LoggerGTest.cpp)
set_property(TARGET LoggerGTest PROPERTY CXX_STANDARD 17)
target_include_directories(LoggerGTest PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../src/)
target_link_libraries(LoggerGTest GTest::gtest_main -lSadaLogger)

include(GoogleTest)
gtest_discover_tests(LoggerGTest)

add_test(LoggerGTest LoggerGTest)

add_executable(LoggerTest LoggerTest.cpp)
set_property(TARGET LoggerTest PROPERTY CXX_STANDARD 17)
target_include_directories(LoggerTest PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../src/)
target_link_libraries(LoggerTest -lSadaLogger)
