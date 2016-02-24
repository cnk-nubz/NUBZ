#ifndef SERVER_IO_INPUT__CREATE_SORT_QUESTION_REQUEST__H
#define SERVER_IO_INPUT__CREATE_SORT_QUESTION_REQUEST__H

#include <string>
#include <vector>

#include <boost/optional.hpp>

#include <communication/structs_types.h>

#include <server/io/MultipleChoiceQuestion.h>

namespace server {
namespace io {
namespace input {

struct CreateSortQuestionRequest {
    CreateSortQuestionRequest(const communication::CreateSortQuestionRequest &thrift);

    boost::optional<std::string> name;
    std::string question;
    std::vector<std::string> options;
};
}
}
}

#endif