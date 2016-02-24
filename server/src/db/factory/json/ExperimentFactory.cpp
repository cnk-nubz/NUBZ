#include <db/db_info.h>

#include <boost/lexical_cast.hpp>

#include "ExperimentFactory.h"
#include "commons.h"

namespace db {
namespace factory {
namespace json {

using namespace info::experiments;
using namespace rapidjson;

const std::vector<std::string> &ExperimentFactory::fieldsOrder() noexcept {
    static const std::vector<std::string> order = {colId, colName, colDocument};
    return order;
}

namespace {
Value createSurvey(const Experiment::Survey &survey, Document::AllocatorType &allocator);
}

std::string ExperimentFactory::createJson(const Experiment &experiment) {
    static const auto keyActions = toStrAdapter(field0_Actions);
    static const auto keyBreakActions = toStrAdapter(field1_BreakActions);
    static const auto keySurveyBefore = toStrAdapter(field2_SurveyBefore);
    static const auto keySurveyAfter = toStrAdapter(field2_SurveyAfter);

    Document document;
    auto &allocator = document.GetAllocator();

    auto jsonActions = createTrivialArray(experiment.actions, allocator);
    auto jsonBreakActions = createTrivialArray(experiment.breakActions, allocator);
    auto jsonSurveyBefore = createSurvey(experiment.surveyBefore, allocator);
    auto jsonSurveyAfter = createSurvey(experiment.surveyAfter, allocator);

    Value json(kObjectType);
    json.AddMember(keyActions, jsonActions, allocator);
    json.AddMember(keyBreakActions, jsonBreakActions, allocator);
    json.AddMember(keySurveyBefore, jsonSurveyBefore, allocator);
    json.AddMember(keySurveyAfter, jsonSurveyAfter, allocator);
    return jsonToString(json);
}

namespace {
Value createSurvey(const Experiment::Survey &survey, Document::AllocatorType &allocator) {
    static const auto keyOrder = toStrAdapter(field20_TypesOrder);
    static const auto keySimpleQuestions = toStrAdapter(field21_SimpleQuestions);
    static const auto keyMultipleChoiceQuestions = toStrAdapter(field22_MultipleChoiceQuestions);
    static const auto keySortQuestions = toStrAdapter(field23_SortQuestions);

    auto jsonOrder = createTrivialArray(survey.order, allocator);
    auto jsonSimpleQuestions = createTrivialArray(survey.simpleQuestions, allocator);
    auto jsonMultipleChoiceQuestions =
        createTrivialArray(survey.multipleChoiceQuestions, allocator);
    auto jsonSortQuestions = createTrivialArray(survey.sortQuestions, allocator);

    Value json(kObjectType);
    json.AddMember(keyOrder, jsonOrder, allocator);
    json.AddMember(keySimpleQuestions, jsonSimpleQuestions, allocator);
    json.AddMember(keyMultipleChoiceQuestions, jsonMultipleChoiceQuestions, allocator);
    json.AddMember(keySortQuestions, jsonSortQuestions, allocator);
    return json;
}
}

namespace {
Experiment::Survey parseSurvey(const Value &json);
}

Experiment ExperimentFactory::create(
    const std::vector<boost::optional<std::string>> &raw) noexcept {
    assert(raw.size() == fieldsOrder().size());
    assert(std::all_of(raw.begin(), raw.end(), [](const auto &e) { return (bool)e; }));

    Experiment experiment;
    assert(boost::conversion::try_lexical_convert(raw[0].value(), experiment.ID));
    experiment.name = raw[1].value();

    Document json = parseJson(raw[2].value());
    experiment.actions = parse(parseIntArray, json, field0_Actions);
    experiment.breakActions = parse(parseIntArray, json, field1_BreakActions);
    experiment.surveyBefore = parse(parseSurvey, json, field2_SurveyBefore);
    experiment.surveyAfter = parse(parseSurvey, json, field2_SurveyAfter);
    return experiment;
}

namespace {
Experiment::Survey parseSurvey(const Value &json) {
    assert(json.IsObject());

    Experiment::Survey survey;
    for (auto t : parse(parseIntArray, json, field20_TypesOrder)) {
        survey.order.push_back(static_cast<Experiment::Survey::QuestionType>(t));
    }
    survey.simpleQuestions = parse(parseIntArray, json, field21_SimpleQuestions);
    survey.multipleChoiceQuestions = parse(parseIntArray, json, field22_MultipleChoiceQuestions);
    survey.sortQuestions = parse(parseIntArray, json, field23_SortQuestions);
    return survey;
}
}
}
}
}