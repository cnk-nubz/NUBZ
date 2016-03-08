set(SOURCES
    Actions.cpp
    SimpleQuestions.cpp
    MultipleChoiceQuestions.cpp
    SortQuestions.cpp
    Exhibits.cpp
    Counters.cpp
    MapImages.cpp
    Experiments.cpp
    Reports.cpp
)

add_library(repository ${SOURCES})

target_link_libraries(repository db utils)