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
    }
}

communication::QuestionType::type Experiment::Survey::QuestionTypeToThrift(
    const Survey::QuestionType &type) {
    switch (type) {
        case Experiment::Survey::QuestionType::Simple:
            return communication::QuestionType::type::SIMPLE;
    }
}

communication::Survey Experiment::Survey::toThrift() const {
    communication::Survey thrift;
    ::utils::transform(questionsOrder, thrift.questionsOrder, QuestionTypeToThrift);
    thrift.simpleQuestions = ioToThrift(simpleQuestions);
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