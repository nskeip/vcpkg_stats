cmake_minimum_required(VERSION 3.21)
project(vcpkg_stats)

set(CMAKE_CXX_STANDARD 17)

add_executable(vcpkg_stats main.cpp)

find_package(args)
find_package(jsoncpp CONFIG REQUIRED)

target_link_libraries(
        vcpkg_stats
        PRIVATE
        taywee::args
        jsoncpp_object
        jsoncpp_static
        JsonCpp::JsonCpp
)
