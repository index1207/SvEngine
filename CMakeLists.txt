cmake_minimum_required(VERSION 3.26)
project(SvEngine)


include_directories(netcpp/include)

link_directories(lib/netcpp/${CMAKE_BUILD_TYPE})
link_libraries(netcpp wsock32 ws2_32)

set(CMAKE_CXX_STANDARD 20)
add_executable(SvEngine src/main.cpp)
