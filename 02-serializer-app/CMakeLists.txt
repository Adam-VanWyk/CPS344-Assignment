cmake_minimum_required(VERSION 3.5)
project(sockets)

find_package(Threads REQUIRED)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SERIALIZER_FILES
  src/test_serializer.cpp
  src/bakery.cpp
)

set(CLIENT_FILES
  src/client.cpp
)

set(SERVER_FILES
  src/server.cpp
  src/bakery.cpp
)

add_executable(test_serializer ${SERIALIZER_FILES})
target_include_directories(test_serializer PRIVATE include)

add_executable(client ${CLIENT_FILES})
target_include_directories(client PRIVATE include)
target_include_directories(client PRIVATE third_party/asio-1.18.0/include)
target_link_libraries(client PRIVATE Threads::Threads)

add_executable(server ${SERVER_FILES})
target_include_directories(server PRIVATE include)
target_include_directories(server PRIVATE third_party/asio-1.18.0/include)
target_link_libraries(server PRIVATE Threads::Threads)
