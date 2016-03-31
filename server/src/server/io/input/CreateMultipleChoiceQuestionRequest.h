#ifndef SERVER_IO_INPUT__CREATE_MULTIPLE_CHOICE_QUESTION_REQUEST__H
#define SERVER_IO_INPUT__CREATE_MULTIPLE_CHOICE_QUESTION_REQUEST__H

#include <string>
#include <vector>

#include <boost/optional.hpp>

#include <communication/structs_types.h>

namespace server {
namespace io {
namespace input {

struct CreateMultipleChoiceQuestionRequest {
    CreateMultipleChoiceQuestionRequest(
        const communication::CreateMultipleChoiceQuestionRequest &thrift);

    const std::string name;
    const std::string question;
    const std::vector<std::string> options;
    const bool singleAnswer;
};
}
}
}

#endif