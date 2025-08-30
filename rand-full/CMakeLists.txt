cmake_minimum_required(VERSION 3.16)
project(rand-full)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(rand-full main.cpp)

target_link_libraries(rand-full Qt6::Widgets)