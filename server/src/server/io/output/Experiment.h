#ifndef SERVER_IO_OUTPUT__EXPERIMENT__H
#define SERVER_IO_OUTPUT__EXPERIMENT__H

#include <cstdint>
#include <string>
#include <vector>

#include <communication/structs_types.h>

#include <server/io/QuestionsList.h>
#include <server/io/SimpleQuestion.h>
#include <server/io/MultipleChoiceQuestion.h>
#include <server/io/Action.h>

namespace server {
namespace io {
namespace output {

struct Experiment {
    using thrift_t = communication::Experiment;

    Experiment() = default;
    thrift_t toThrift() const;

    std::int32_t ID;
    std::string name;

    std::vector<Action> exhibitActions;
    std::vector<Action> breakActions;

    QuestionsList surveyBefore;
    QuestionsList surveyAfter;
};
}
}
}

#endif