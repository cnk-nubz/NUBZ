#include <utils/fp_algorithm.h>

#include "QuestionsIdsList.h"

namespace server {
namespace io {
namespace input {

QuestionsIdsList::QuestionsIdsList(const communication::QuestionsIdsList &thrift)
    : simpleQuestions(thrift.simpleQuestions),
      multipleChoiceQuestions(thrift.multipleChoiceQuestions),
      sortQuestions(thrift.sortQuestions) {
    ::utils::transform(thrift.questionsOrder, questionsOrder, questionTypeFromThrift);
}
}
}
}