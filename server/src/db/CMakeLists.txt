file(GLOB SOURCES
    *.cpp
    factory/*.cpp
    factory/json/*.cpp
    struct/*.cpp
    command/*.cpp
)

add_library(db ${SOURCES})

target_link_libraries(db utils)