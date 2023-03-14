cmake_minimum_required(VERSION 3.16)

project(SadaLogger)

set(SOURCES
${PROJECT_SOURCE_DIR}/src/Logger.cpp)

add_library(SadaLogger SHARED ${SOURCES})
set_property(TARGET SadaLogger PROPERTY CXX_STANDARD 17)
target_link_libraries(SadaLogger -lpthread)
target_include_directories(SadaLogger PUBLIC ${PROJECT_SOURCE_DIR}/src/)

enable_testing()
add_subdirectory(tests)
