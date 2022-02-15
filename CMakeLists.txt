cmake_minimum_required(VERSION 3.21)
project(vcpkg_stats)

set(CMAKE_CXX_STANDARD 17)

add_executable(vcpkg_stats main.cpp)

find_package(args)
target_link_libraries(vcpkg_stats PRIVATE taywee::args)
