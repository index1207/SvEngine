cmake_minimum_required(VERSION 3.26)
project(SvEngine)


include_directories(include)
include_directories(netcpp/include)

link_directories(netcpp/cmake-build-${CMAKE_BUILD_TYPE})
link_libraries(netcpp wsock32 ws2_32)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd /Zi /RTC1")
set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} /DEBUG")

set(CMAKE_CXX_STANDARD 20)

add_executable(SvEngine src/main.cpp)

target_sources(SvEngine PUBLIC
        src/Listener.cpp
)