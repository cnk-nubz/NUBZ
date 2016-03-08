#include <utils/fp_algorithm.h>

#include <db/json/json.h>

#include "Experiments.h"

/*
 * columns:
 * - id
 * - name
 * - state
 * - startDate
 * - finishDate
 * - content (json)
 *
 * content = {actions, breakActions, surveyBefore, surveyAfter}
 * actions, breakActions = [int, ...]
 * survey = {typesOrder, simpleQuestions, multipleChoiceQuestions, sortQuestions}
 * typesOrder, simpleQuestions, multipleChoiceQuestions, sortQuestions = [int, ...]
 */

namespace db {
namespace table {

const std::string Experiments::tableName = "experiments";
const std::string Experiments::ColumnId::name = "id";
const std::string Experiments::ColumnName::name = "name";
const std::string Experiments::ColumnState::name = "state";
const std::string Experiments::ColumnStartDate::name = "start_date";
const std::string Experiments::ColumnFinishDate::name = "finish_date";
const std::string Experiments::ColumnContent::name = "content";

namespace {
struct RootKeys {
    static const std::string actions;
    static const std::string breakActions;
    static const std::string surveyBefore;
    static const std::string surveyAfter;
    struct SurveyKeys {
        static const std::string typesOrder;
        static const std::string simpleQuestions;
        static const std::string multipleChoiceQuestions;
        static const std::string sortQuestions;
    };
};

const std::string RootKeys::actions = "actions";
const std::string RootKeys::breakActions = "breakActions";
const std::string RootKeys::surveyBefore = "surveyBefore";
const std::string RootKeys::surveyAfter = "surveyAfter";

const std::string RootKeys::SurveyKeys::typesOrder = "typesOrder";
const std::string RootKeys::SurveyKeys::simpleQuestions = "simpleQuestions";
const std::string RootKeys::SurveyKeys::multipleChoiceQuestions = "multipleChoiceQuestions";
const std::string RootKeys::SurveyKeys::sortQuestions = "sortQuestions";
}

const Experiments::ColumnId Experiments::colId{};
const Experiments::ColumnName Experiments::colName{};
const Experiments::ColumnState Experiments::colState{};
const Experiments::ColumnStartDate Experiments::colStartDate{};
const Experiments::ColumnFinishDate Experiments::colFinishDate{};
const Experiments::ColumnContent Experiments::colContent{};

namespace {
using int_vec = std::vector<std::int32_t>;

// returns {actions, breakActions, surveyBefore, surveyAfter>
std::tuple<int_vec, int_vec, Experiments::Row::Survey, Experiments::Row::Survey> parseContent(
    const std::string &jsonStr);
Experiments::Row::Survey parseSurvey(const rapidjson::Value &json);
}

Experiments::Row Experiments::RowFactory::fromDB(const DBOut &dbOut) {
    auto res = Experiments::Row{};
    res.ID = std::get<ValueId>(dbOut).value;
    res.name = std::get<ValueName>(dbOut).value;
    res.state = std::get<ValueState>(dbOut).value;
    res.startDate = std::get<ValueStartDate>(dbOut).value;
    res.finishDate = std::get<ValueFinishDate>(dbOut).value;
    std::tie(res.actions, res.breakActions, res.surveyBefore, res.surveyAfter) =
        parseContent(std::get<ValueContent>(dbOut).value);
    return res;
}

namespace {
using namespace db::json;

std::tuple<int_vec, int_vec, Experiments::Row::Survey, Experiments::Row::Survey> parseContent(
    const std::string &jsonStr) {
    using Keys = RootKeys;
    auto json = parseJson(jsonStr);
    return std::make_tuple(parseIntArray(getNode(json, Keys::actions)),
                           parseIntArray(getNode(json, Keys::breakActions)),
                           parseSurvey(getNode(json, Keys::surveyBefore)),
                           parseSurvey(getNode(json, Keys::surveyAfter)));
}

Experiments::Row::Survey parseSurvey(const rapidjson::Value &json) {
    using Keys = RootKeys::SurveyKeys;
    auto res = Experiments::Row::Survey{};
    res.typesOrder = parseIntArray(getNode(json, Keys::typesOrder));
    res.simpleQuestions = parseIntArray(getNode(json, Keys::simpleQuestions));
    res.multipleChoiceQuestions = parseIntArray(getNode(json, Keys::multipleChoiceQuestions));
    res.sortQuestions = parseIntArray(getNode(json, Keys::sortQuestions));
    return res;
}
}

namespace {
std::string createContent(const Experiments::Row &row);
rapidjson::Value createSurvey(rapidjson::Document::AllocatorType &allocator,
                              const Experiments::Row::Survey &survey);
}

Experiments::RowFactory::DBIn Experiments::RowFactory::toDB(
    const db::table::Experiments::Row &row) {
    return std::make_tuple(ValueName{row.name},
                           ValueState{row.state},
                           ValueStartDate{row.startDate},
                           ValueFinishDate{row.finishDate},
                           ValueContent{createContent(row)});
}

namespace {
using namespace db::json;

std::string createContent(const Experiments::Row &row) {
    using Keys = RootKeys;
    auto document = rapidjson::Document{};
    auto &allocator = document.GetAllocator();

    auto json = createDictionary(
        allocator,
        std::make_pair(Keys::actions, createIntArray(allocator, row.actions)),
        std::make_pair(Keys::breakActions, createIntArray(allocator, row.breakActions)),
        std::make_pair(Keys::surveyBefore, createSurvey(allocator, row.surveyBefore)),
        std::make_pair(Keys::surveyAfter, createSurvey(allocator, row.surveyAfter)));
    return jsonToString(json);
}

rapidjson::Value createSurvey(rapidjson::Document::AllocatorType &allocator,
                              const Experiments::Row::Survey &survey) {
    using Keys = RootKeys::SurveyKeys;

    auto typesOrder = createIntArray(allocator, survey.typesOrder);
    auto simpleQuestions = createIntArray(allocator, survey.simpleQuestions);
    auto multipleChoiceQuestions = createIntArray(allocator, survey.multipleChoiceQuestions);
    auto sortQuestions = createIntArray(allocator, survey.sortQuestions);

    return createDictionary(
        allocator,
        std::make_pair(Keys::typesOrder, std::move(typesOrder)),
        std::make_pair(Keys::simpleQuestions, std::move(simpleQuestions)),
        std::make_pair(Keys::multipleChoiceQuestions, std::move(multipleChoiceQuestions)),
        std::make_pair(Keys::sortQuestions, std::move(sortQuestions)));
}
}
}
}
