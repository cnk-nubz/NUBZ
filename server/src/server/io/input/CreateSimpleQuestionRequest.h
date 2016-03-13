#ifndef SERVER_IO_INPUT__CREATE_SIMPLE_QUESTION_REQUEST__H
#define SERVER_IO_INPUT__CREATE_SIMPLE_QUESTION_REQUEST__H

#include <string>

#include <boost/optional.hpp>

#include <communication/structs_types.h>

#include <server/io/output/SimpleQuestion.h>

namespace server {
namespace io {
namespace input {

struct CreateSimpleQuestionRequest {
    CreateSimpleQuestionRequest(const communication::CreateSimpleQuestionRequest &thrift);

    boost::optional<std::string> name;
    std::string question;
    output::SimpleQuestion::AnswerType answerType;
};
}
}
}

#endif