cmake_minimum_required(VERSION 3.5...3.28)
project(sockets) 

find_package(Threads REQUIRED)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(tcp_client src/tcp_client.cpp)
target_include_directories(tcp_client PRIVATE include)
target_include_directories(tcp_client PRIVATE third_party/asio-1.18.0/include)
target_link_libraries(tcp_client PRIVATE Threads::Threads)

add_executable(tcp_server src/tcp_server.cpp)
target_include_directories(tcp_server PRIVATE include)
target_include_directories(tcp_server PRIVATE third_party/asio-1.18.0/include)
target_link_libraries(tcp_server PRIVATE Threads::Threads)

