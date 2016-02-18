#include <utils/fp_algorithm.h>

#include <server/io/utils.h>

#include "Experiment.h"

namespace server {
namespace io {
namespace output {

Experiment::Survey::QuestionType Experiment::Survey::QuestionTypeFromThrift(
    const communication::QuestionType::type &thrift) {
    switch (thrift) {
        case communication::QuestionType::type::SIMPLE:
            return Experiment::Survey::QuestionType::Simple;
        case communication::QuestionType::type::MULTIPLE_CHOICE:
            return Experiment::Survey::QuestionType::MultipleChoice;
    }
}

communication::QuestionType::type Experiment::Survey::QuestionTypeToThrift(
    const Survey::QuestionType &type) {
    switch (type) {
        case Experiment::Survey::QuestionType::Simple:
            return communication::QuestionType::type::SIMPLE;
        case Experiment::Survey::QuestionType::MultipleChoice:
            return communication::QuestionType::type::MULTIPLE_CHOICE;
    }
}

communication::Survey Experiment::Survey::toThrift() const {
    communication::Survey thrift;
    ::utils::transform(questionsOrder, thrift.questionsOrder, QuestionTypeToThrift);
    thrift.simpleQuestions = ioToThrift(simpleQuestions);
    thrift.multipleChoiceQuestions = ioToThrift(multipleChoiceQuestions);
    return thrift;
}

communication::Experiment Experiment::toThrift() const {
    communication::Experiment res;
    res.experimentId = ID;
    res.name = name;
    res.exhibitActions = ioToThrift(exhibitActions);
    res.breakActions = ioToThrift(breakActions);
    res.surveyBefore = surveyBefore.toThrift();
    res.surveyAfter = surveyAfter.toThrift();
    return res;
}
}
}
}