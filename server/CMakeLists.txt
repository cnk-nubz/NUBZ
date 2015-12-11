cmake_minimum_required (VERSION 2.6)
project (zpp-cnk)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

find_package(Boost COMPONENTS system program_options filesystem REQUIRED)
find_package(Thrift REQUIRED)
find_package(PostgreSQL REQUIRED)
find_package(Pqxx REQUIRED)
find_package(RapidJSON REQUIRED)
find_package(ImageMagick COMPONENTS Magick++ REQUIRED)
find_package(OpenSSL)

include_directories("src")
include_directories(${Boost_INCLUDE_DIR})
include_directories(${Thrift_INCLUDE_DIR})
include_directories(${PostgreSQL_INCLUDE_DIR})
include_directories(${Pqxx_INCLUDE_DIR})
include_directories(${ImageMagick_INCLUDE_DIRS})

link_directories(${Boost_LIBRARY_DIRS})
link_directories(${Thrift_LIBRARY_DIRS})
link_directories(${PostgreSQL_LIBRARY_DIRS})
link_directories(${Pqxx_LIBRARY_DIRS})

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -Wextra -std=c++11 -DMAGICKCORE_HDRI_ENABLE=0 -DMAGICKCORE_QUANTUM_DEPTH=16")

file(GLOB SOURCES
    src/*.cpp
    src/communication/*.cpp
    src/command/*.cpp
    src/external/*.cpp
    src/image/*.cpp
    src/utils/*.cpp
    src/io/*.cpp
    src/io/input/*.cpp
    src/io/output/*.cpp
    src/db/*.cpp
    src/db/command/*.cpp
    src/db/factory/*.cpp
    src/db/struct/*.cpp
)

add_executable(main ${SOURCES})

target_link_libraries(main
    ssl
    pthread
    thrift
    ${Boost_LIBRARIES}
    ${PostgreSQL_LIBRARIES}
    ${Pqxx_LIBRARIES}
    ${ImageMagick_LIBRARIES}
)
