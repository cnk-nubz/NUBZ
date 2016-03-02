set(SOURCES
    Actions.cpp
    SimpleQuestions.cpp
)

add_library(repository ${SOURCES})

target_link_libraries(repository db utils)