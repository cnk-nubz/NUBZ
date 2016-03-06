#ifndef DB__DB_INFO__H
#define DB__DB_INFO__H

#include <string>

namespace db {
namespace info {

namespace reports {
static const std::string tableName = "reports";

static const std::string colId = "id";
static const std::string colExperimentId = "experiment_id";
static const std::string colDocument = "doc";

// json
// field[level]_[name]
static const std::string field0_History = "history";

static const std::string field00_ExhibitId = "exhibitId";
static const std::string field01_DurationInSecs = "durationInSecs";
static const std::string field02_Actions = "actions";

static const std::string field1_SurveyBefore = "surveyBefore";
static const std::string field1_SurveyAfter = "surveyAfter";

static const std::string field10_SimpleQuestions = "simpleQuestions";
static const std::string field100_Answer = "answer";
static const std::string field11_MultipleChoiceQuestions = "multipleChoiceQuestions";
static const std::string field110_Answer = "answer";
static const std::string field12_SortQuestions = "sortQuestions";
static const std::string field120_Answer = "answer";

}  // reports

namespace experiments {
namespace current {
static const std::string tableName = "active_experiment";
static const std::string colId = "id";
}  // current

static const std::string tableName = "experiments";

static const std::string colId = "id";
static const std::string colName = "name";
static const std::string colDocument = "doc";

// json
// field[level]_[name]
static const std::string field0_Actions = "actions";
static const std::string field1_BreakActions = "breakActions";

static const std::string field2_SurveyBefore = "surveyBefore";
static const std::string field2_SurveyAfter = "surveyAfter";
static const std::string field20_TypesOrder = "typesOrder";
static const std::string field21_SimpleQuestions = "simpleQuestions";
static const std::string field22_MultipleChoiceQuestions = "multipleChoiceQuestions";
static const std::string field23_SortQuestions = "sortQuestions";
}  // experiments
}
}

#endif