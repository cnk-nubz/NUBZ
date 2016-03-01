set(SOURCES
    Actions.cpp
)

add_library(repository ${SOURCES})

target_link_libraries(repository db utils)