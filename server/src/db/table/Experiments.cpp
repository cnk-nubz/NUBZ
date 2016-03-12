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

const std::string Experiments::FieldID::columnName = "id";
const std::string Experiments::FieldName::columnName = "name";
const std::string Experiments::FieldState::columnName = "state";
const std::string Experiments::FieldStartDate::columnName = "start_date";
const std::string Experiments::FieldFinishDate::columnName = "finish_date";
const std::string Experiments::FieldContent::columnName = "content";

namespace {
struct RootKeys {
    static const char *actions;
    static const char *breakActions;
    static const char *surveyBefore;
    static const char *surveyAfter;
    struct SurveyKeys {
        static const char *typesOrder;
        static const char *simpleQuestions;
        static const char *multipleChoiceQuestions;
        static const char *sortQuestions;
    };
};

const char *RootKeys::actions = "actions";
const char *RootKeys::breakActions = "breakActions";
const char *RootKeys::surveyBefore = "surveyBefore";
const char *RootKeys::surveyAfter = "surveyAfter";

const char *RootKeys::SurveyKeys::typesOrder = "typesOrder";
const char *RootKeys::SurveyKeys::simpleQuestions = "simpleQuestions";
const char *RootKeys::SurveyKeys::multipleChoiceQuestions = "multipleChoiceQuestions";
const char *RootKeys::SurveyKeys::sortQuestions = "sortQuestions";

Experiments::ContentData::Survey parseSurvey(const rapidjson::Value &json);
rapidjson::Value createSurvey(rapidjson::Document::AllocatorType &allocator,
                              const Experiments::ContentData::Survey &survey);
}

using namespace db::json;

Experiments::ContentData::ContentData(const std::string &jsonStr) {
    using Keys = RootKeys;
    auto json = parseJson(jsonStr);

    actions = parseIntArray(getNode(json, Keys::actions));
    breakActions = parseIntArray(getNode(json, Keys::breakActions));
    surveyBefore = parseSurvey(getNode(json, Keys::surveyBefore));
    surveyAfter = parseSurvey(getNode(json, Keys::surveyAfter));
}

Experiments::ContentData::operator std::string() const {
    using Keys = RootKeys;

    auto document = rapidjson::Document{};
    auto &allocator = document.GetAllocator();

    auto json = createDictionary(
        allocator,
        std::make_pair(Keys::actions, createIntArray(allocator, actions)),
        std::make_pair(Keys::breakActions, createIntArray(allocator, breakActions)),
        std::make_pair(Keys::surveyBefore, createSurvey(allocator, surveyBefore)),
        std::make_pair(Keys::surveyAfter, createSurvey(allocator, surveyAfter)));
    return jsonToString(json);
}

namespace {
Experiments::ContentData::Survey parseSurvey(const rapidjson::Value &json) {
    using Keys = RootKeys::SurveyKeys;
    auto res = Experiments::ContentData::Survey{};
    res.typesOrder = parseIntArray(getNode(json, Keys::typesOrder));
    res.simpleQuestions = parseIntArray(getNode(json, Keys::simpleQuestions));
    res.multipleChoiceQuestions = parseIntArray(getNode(json, Keys::multipleChoiceQuestions));
    res.sortQuestions = parseIntArray(getNode(json, Keys::sortQuestions));
    return res;
}

rapidjson::Value createSurvey(rapidjson::Document::AllocatorType &allocator,
                              const Experiments::ContentData::Survey &survey) {
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
