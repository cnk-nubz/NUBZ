#ifndef SERVER_IO_INPUT__CREATE_SORT_QUESTION_REQUEST__H
#define SERVER_IO_INPUT__CREATE_SORT_QUESTION_REQUEST__H

#include <string>
#include <vector>

#include <boost/optional.hpp>

#include <communication/structs_types.h>

namespace server {
namespace io {
namespace input {

struct CreateSortQuestionRequest {
    CreateSortQuestionRequest(const communication::CreateSortQuestionRequest &thrift);

    const std::string name;
    const std::string question;
    const std::vector<std::string> options;
};
}
}
}

#endif