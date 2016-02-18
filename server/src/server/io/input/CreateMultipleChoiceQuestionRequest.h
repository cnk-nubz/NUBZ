#ifndef SERVER_IO_INPUT__CREATE_MULTIPLE_CHOICE_QUESTION_REQUEST__H
#define SERVER_IO_INPUT__CREATE_MULTIPLE_CHOICE_QUESTION_REQUEST__H

#include <string>
#include <vector>

#include <boost/optional.hpp>

#include <communication/structs_types.h>

#include <server/io/MultipleChoiceQuestion.h>

namespace server {
namespace io {
namespace input {

struct CreateMultipleChoiceQuestionRequest {
    CreateMultipleChoiceQuestionRequest(
        const communication::CreateMultipleChoiceQuestionRequest &thrift);

    boost::optional<std::string> name;
    std::string question;
    bool singleAnswer;
    std::vector<std::string> options;
};
}
}
}

#endif