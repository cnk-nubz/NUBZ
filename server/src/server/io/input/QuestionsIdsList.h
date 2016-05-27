#ifndef SERVER_IO_INPUT__QUESTIONS_IDS_LIST__H
#define SERVER_IO_INPUT__QUESTIONS_IDS_LIST__H

#include <cstdint>
#include <vector>

#include <communication/structs_types.h>

#include <server/io/QuestionType.h>

namespace server {
namespace io {
namespace input {

struct QuestionsIdsList {
    QuestionsIdsList(const communication::QuestionsIdsList &thrift);

    std::vector<QuestionType> questionsOrder;
    std::vector<std::int32_t> simpleQuestions;
    std::vector<std::int32_t> multipleChoiceQuestions;
    std::vector<std::int32_t> sortQuestions;
};
}
}
}

#endif