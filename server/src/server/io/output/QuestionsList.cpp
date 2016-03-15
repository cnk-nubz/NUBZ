#include <utils/fp_algorithm.h>

#include <server/io/utils.h>

#include "QuestionsList.h"

namespace server {
namespace io {
namespace output {

QuestionsList::QuestionsList(const repository::Experiment::Survey &repo)
    : simpleQuestions(std::vector<output::SimpleQuestion>(repo.simpleQuestions.begin(),
                                                          repo.simpleQuestions.end())),
      multipleChoiceQuestions(std::vector<output::MultipleChoiceQuestion>(
          repo.multipleChoiceQuestions.begin(), repo.multipleChoiceQuestions.end())),
      sortQuestions(
          std::vector<output::SortQuestion>(repo.sortQuestions.begin(), repo.sortQuestions.end())) {
    for (auto &type : repo.typesOrder) {
        switch (type) {
            case repository::Experiment::Survey::QuestionType::Simple:
                questionsOrder.push_back(QuestionType::Simple);
                break;
            case repository::Experiment::Survey::QuestionType::Sort:
                questionsOrder.push_back(QuestionType::Sort);
                break;
            case repository::Experiment::Survey::QuestionType::MultipleChoice:
                questionsOrder.push_back(QuestionType::MultipleChoice);
                break;
        }
    }
}

communication::QuestionsList QuestionsList::toThrift() const {
    auto res = communication::QuestionsList{};
    ::utils::transform(questionsOrder, res.questionsOrder, questionTypeToThrift);
    res.simpleQuestions = ioToThrift(simpleQuestions);
    res.multipleChoiceQuestions = ioToThrift(multipleChoiceQuestions);
    res.sortQuestions = ioToThrift(sortQuestions);
    return res;
}
}
}
}