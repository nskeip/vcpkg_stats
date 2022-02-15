cmake_minimum_required(VERSION 3.21)
project(vcpkg_stats)

set(CMAKE_CXX_STANDARD 20)

add_executable(vcpkg_stats main.cpp)

find_package(args)
find_package(jsoncpp CONFIG REQUIRED)

include(FetchContent)

FetchContent_Declare(
        Cpp20InternetClient
        GIT_REPOSITORY https://github.com/avocadoboi/cpp20-internet-client.git
        GIT_TAG v2.1.3
)
FetchContent_MakeAvailable(Cpp20InternetClient)

target_link_libraries(
        vcpkg_stats
        PRIVATE
        taywee::args
        jsoncpp_object
        jsoncpp_static
        JsonCpp::JsonCpp
        Cpp20InternetClient::cpp20_internet_client
)
