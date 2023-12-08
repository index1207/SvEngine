cmake_minimum_required(VERSION 3.9)
project(SvEngine)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)
include_directories(netcpp/include)

link_directories(netcpp/cmake-build-${CMAKE_BUILD_TYPE})
link_libraries(netcpp ws2_32)

add_executable(SvEngine src/main.cpp)

target_sources(SvEngine PRIVATE
        src/Listener.cpp
)