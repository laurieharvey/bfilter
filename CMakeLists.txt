cmake_minimum_required(VERSION 3.0.0)
project(bfilter VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

add_executable(test test/test.cpp)

target_link_libraries(test libgtest.a)
target_link_libraries(test libgtest_main.a)
target_link_libraries(test libpthread.so)
