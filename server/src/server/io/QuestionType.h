#ifndef SERVER_IO__QUESTION_TYPE__H
#define SERVER_IO__QUESTION_TYPE__H

#include <communication/structs_types.h>

namespace server {
namespace io {

enum class QuestionType {
    Simple,
    MultipleChoice,
    Sort,
};

QuestionType questionTypeFromThrift(const communication::QuestionType::type &thrift);
communication::QuestionType::type questionTypeToThrift(const QuestionType &type);
}
}

#endif