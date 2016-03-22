cmake_minimum_required (VERSION 3.1)
project (zpp-cnk)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -Wextra -DMAGICKCORE_HDRI_ENABLE=0 -DMAGICKCORE_QUANTUM_DEPTH=16")

find_package(Boost COMPONENTS system program_options filesystem date_time REQUIRED)
find_package(Thrift REQUIRED)
find_package(PostgreSQL REQUIRED)
find_package(Pqxx REQUIRED)
find_package(RapidJSON REQUIRED)
find_package(ImageMagick COMPONENTS Magick++ REQUIRED)
find_package(OpenSSL)

include_directories("src")
include_directories(SYSTEM "external")
include_directories(${Boost_INCLUDE_DIR})
include_directories(${Thrift_INCLUDE_DIR})
include_directories(${PostgreSQL_INCLUDE_DIR})
include_directories(${Pqxx_INCLUDE_DIR})
include_directories(SYSTEM ${ImageMagick_INCLUDE_DIRS})

link_directories(${Boost_LIBRARY_DIRS})
link_directories(${Thrift_LIBRARY_DIRS})
link_directories(${PostgreSQL_LIBRARY_DIRS})
link_directories(${Pqxx_LIBRARY_DIRS})

add_subdirectory(src/utils)
add_subdirectory(src/db)
add_subdirectory(src/repository)
add_subdirectory(src/communication)
add_subdirectory(src/server)

set(SOURCES
    src/main.cpp
)

add_executable(main ${SOURCES})

target_link_libraries(
    main
    server
    communication
    repository
    db
    utils
    ssl
    thrift
    thriftnb
    event
    pthread
    ${Boost_LIBRARIES}
    ${PostgreSQL_LIBRARIES}
    ${Pqxx_LIBRARIES}
    ${ImageMagick_LIBRARIES}
)
