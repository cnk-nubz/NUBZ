set(SOURCES
    Config.cpp
    FileHandler.cpp
    ImageProcessor.cpp
    excel/column/Column.cpp
    excel/ExcelProcessor.cpp
    excel/Experiment.cpp
    excel/History.cpp
    excel/Report.cpp
    excel/Survey.cpp
    excel/WrapperFactory.cpp
)

add_library(utils ${SOURCES})