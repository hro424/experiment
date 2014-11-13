cmake_minimum_required(VERSION 3.0)
project(unix)

find_package(Threads)

add_compile_options(-std=c++11 -std=c11 -Wall)

add_executable(dgram_client dgram_client.c)
add_executable(dgram_server dgram_server.c)
add_executable(stream_client stream_client.c)
add_executable(stream_server stream_server.c)
add_executable(tcpd tcpd.cc)
add_executable(thread_server thread_server.c)

target_link_libraries(dgram_server ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(thread_server ${CMAKE_THREAD_LIBS_INIT})
