#ifndef REPOSITORY__EXPERIMENTS__H
#define REPOSITORY__EXPERIMENTS__H

#include <cstdint>
#include <string>

#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/optional.hpp>

#include <db/DatabaseSession.h>

#include "Actions.h"
#include "MultipleChoiceQuestions.h"
#include "SimpleQuestions.h"
#include "SortQuestions.h"

namespace repository {

class Experiments {
public:
    struct _Experiment {
        enum QuestionType : std::int32_t { Simple = 0, MultipleChoice = 1, Sort = 2 };

        std::int32_t ID;
        std::string name;
        boost::optional<boost::gregorian::date> startDate;
        boost::optional<boost::gregorian::date> finishDate;
    };

    struct Experiment : _Experiment {
        struct Survey {
            using QuestionType = _Experiment::QuestionType;

            // defines questions order
            std::vector<QuestionType> typesOrder;

            std::vector<SimpleQuestion> simpleQuestions;
            std::vector<MultipleChoiceQuestion> multipleChoiceQuestions;
            std::vector<SortQuestion> sortQuestions;
        };

        std::vector<Action> actions;
        std::vector<Action> breakActions;

        Survey surveyBefore;
        Survey surveyAfter;
    };

    // contains ids instead of whole objects
    struct LazyExperiment : _Experiment {
        struct Survey {
            using QuestionType = _Experiment::QuestionType;

            // defines questions order
            std::vector<QuestionType> typesOrder;

            // unique, foreign keys
            std::vector<std::int32_t> simpleQuestions;

            // unique, foreign keys
            std::vector<std::int32_t> multipleChoiceQuestions;

            // unique, foreign keys
            std::vector<std::int32_t> sortQuestions;
        };

        // unique, foreign keys
        std::vector<std::int32_t> actions;

        // unique, foreign keys
        std::vector<std::int32_t> breakActions;

        Survey surveyBefore;
        Survey surveyAfter;
    };

    Experiments(db::DatabaseSession &session);

    boost::optional<Experiment> get(std::int32_t ID);
    boost::optional<LazyExperiment> getLazy(std::int32_t ID);

    // throws InvalidData for several reasons:
    // - invalid id
    // - id of finished experiment (it's impossible to activate finished experiment)
    // - there is currently active experiment
    void start(std::int32_t ID);

    boost::optional<Experiment> getActive();
    boost::optional<LazyExperiment> getLazyActive();
    void finishActive();

    std::vector<LazyExperiment> getAllReady();
    std::vector<LazyExperiment> getAllFinished();

    // ID, startDate and finishDate will be saved in the given struct
    // throws:
    // - InvalidData
    // - DuplicateName
    void insert(LazyExperiment *experiment);

    // throws:
    // - InvalidData
    // - DuplicateName (in case of new name)
    void update(const LazyExperiment &experiment);

    // throws:
    // - InvalidData (in case of nonexisting ID)
    // - DuplicateName
    void clone(std::int32_t ID, const std::string &name);

private:
    enum State : std::int32_t { Ready = 0, Active = 1, Finished = 2 };

    void insertCheck(const LazyExperiment &experiment);
    void insertExec(LazyExperiment *experiment);

    void updateCheck(const LazyExperiment &experiment);
    void updateExec(const LazyExperiment &experiment);

    void cloneCheck(std::int32_t ID, const std::string &name);
    void cloneExec(std::int32_t ID, const std::string &name);

    State getState(std::int32_t ID);
    std::vector<LazyExperiment> getAllWithState(State state);

    void checkID(std::int32_t ID);
    void checkName(const std::string &name);
    void checkExperiment(const LazyExperiment &experiment);
    void checkActions(const LazyExperiment &experiment);
    void checkSurvey(const LazyExperiment::Survey &survey);
    void checkIds(const std::unordered_set<std::int32_t> &existing,
                  const std::vector<std::int32_t> &choosen) const;
    void checkForDuplicates(std::vector<std::int32_t> ids) const;

    db::DatabaseSession &session;
};

using Experiment = Experiments::Experiment;
}

#endif