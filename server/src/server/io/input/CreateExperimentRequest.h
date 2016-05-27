#ifndef SERVER_IO_INPUT__CREATE_EXPERIMENT_REQUEST__H
#define SERVER_IO_INPUT__CREATE_EXPERIMENT_REQUEST__H

#include <cstdint>
#include <string>

#include <communication/structs_types.h>

#include "QuestionsIdsList.h"

namespace server {
namespace io {
namespace input {

struct CreateExperimentRequest {
    CreateExperimentRequest(const communication::CreateExperimentRequest &thrift);

    std::string name;
    QuestionsIdsList surveyBefore;
    std::vector<std::int32_t> actions;
    std::vector<std::int32_t> breakActions;
    QuestionsIdsList surveyAfter;
};
}
}
}

#endif