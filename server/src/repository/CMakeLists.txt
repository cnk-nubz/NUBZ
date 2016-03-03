set(SOURCES
    Actions.cpp
    SimpleQuestions.cpp
    SortQuestions.cpp
    Exhibits.cpp
    Counters.cpp
)

add_library(repository ${SOURCES})

target_link_libraries(repository db utils)