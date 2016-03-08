file(GLOB SOURCES
    *.cpp
    command/*.cpp
    table/*.cpp
    json/*.cpp
)

add_library(db ${SOURCES})

target_link_libraries(db utils)