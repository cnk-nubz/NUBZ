#ifndef SERVER_IO_OUTPUT__EXPERIMENT__H
#define SERVER_IO_OUTPUT__EXPERIMENT__H

#include <cstdint>
#include <string>
#include <vector>

#include <communication/structs_types.h>

#include <repository/Experiments.h>

#include "Action.h"
#include "MultipleChoiceQuestion.h"
#include "QuestionsList.h"
#include "SimpleQuestion.h"

namespace server {
namespace io {
namespace output {

struct Experiment {
    using thrift_t = communication::Experiment;
    using repo_t = repository::Experiment;

    Experiment() = default;
    thrift_t toThrift() const;

    Experiment(const repo_t &repo);

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