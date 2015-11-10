cmake_minimum_required (VERSION 2.6)
project (zpp-cnk)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

find_package(Boost COMPONENTS system program_options filesystem REQUIRED)
find_package(Thrift REQUIRED)
find_package(OpenSSL)

include_directories("src")
include_directories(${Boost_INCLUDE_DIR})
include_directories(${Thrift_INCLUDE_DIR})

link_directories(${Boost_LIBRARY_DIRS})
link_directories(${Thrift_LIBRARY_DIRS})

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -Wextra -std=c++11")

file(GLOB SOURCES
    src/*.cpp
    src/communication/*.cpp
    src/command/*.cpp
    src/external/*.cpp
    src/io/input/*.cpp
)

add_executable(main ${SOURCES})

target_link_libraries(main
    ssl
    pthread
    thrift
    ${Boost_LIBRARIES}
)