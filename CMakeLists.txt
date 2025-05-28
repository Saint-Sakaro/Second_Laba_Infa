cmake_minimum_required(VERSION 3.10)
project(Lab2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(GTest CONFIG REQUIRED)

add_executable(Tests tests/Tests.cpp)
target_include_directories(Tests PRIVATE include)
target_link_libraries(Tests GTest::gtest GTest::gtest_main)

add_executable(Lab2 src/main.cpp)
target_include_directories(Lab2 PRIVATE include)