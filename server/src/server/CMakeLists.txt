file(GLOB SOURCES
    *.cpp
    command/*.cpp
    utils/*.cpp
    io/*.cpp
    io/input/*.cpp
    io/output/*.cpp
)

add_library(server ${SOURCES})

target_link_libraries(server db utils)