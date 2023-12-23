cmake_minimum_required(VERSION 3.9)
project(SvEngine)

set(CMAKE_CXX_STANDARD 20)
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")

include_directories(include)
include_directories(netcpp/include)

link_directories(netcpp/cmake-build-${CMAKE_BUILD_TYPE})
link_directories(netcpp/out)
link_libraries(netcpp ws2_32)

add_executable(SvEngine src/main.cpp)

target_sources(SvEngine PRIVATE
        src/core/Engine.cpp
        src/core/Server.cpp
        src/core/Client.cpp
        src/core/Packet.cpp
        src/util/Console.cpp
)