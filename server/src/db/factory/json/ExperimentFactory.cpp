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
    static const auto keyActions = toStupidStringAdapter(field0_Actions);
    static const auto keyBreakActions = toStupidStringAdapter(field1_BreakActions);
    static const auto keySurveyBefore = toStupidStringAdapter(field2_SurveyBefore);
    static const auto keySurveyAfter = toStupidStringAdapter(field2_SurveyAfter);

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
    static const auto keyOrder = toStupidStringAdapter(field20_TypesOrder);
    static const auto keySimpleQuestions = toStupidStringAdapter(field21_SimpleQuestions);

    auto jsonOrder = createTrivialArray(survey.order, allocator);
    auto jsonSimpleQuestions = createTrivialArray(survey.simpleQuestions, allocator);

    Value json(kObjectType);
    json.AddMember(keyOrder, jsonOrder, allocator);
    json.AddMember(keySimpleQuestions, jsonSimpleQuestions, allocator);
    return json;
}
}

namespace {
Experiment::Survey parseSurvey(const Value &jsonSurvey);
}

Experiment ExperimentFactory::create(
    const std::vector<boost::optional<std::string>> &raw) noexcept {
    assert(raw.size() == fieldsOrder().size());
    assert(std::all_of(raw.begin(), raw.end(), [](const auto &e) { return (bool)e; }));

    Experiment experiment;
    assert(boost::conversion::try_lexical_convert(raw[0].value(), experiment.ID));
    experiment.name = raw[1].value();

    Document json = parseJson(raw[2].value());
    experiment.actions = parseIntArray(getNode(json, field0_Actions));
    experiment.breakActions = parseIntArray(getNode(json, field1_BreakActions));
    experiment.surveyBefore = parseSurvey(getNode(json, field2_SurveyBefore));
    experiment.surveyAfter = parseSurvey(getNode(json, field2_SurveyAfter));
    return experiment;
}

namespace {
Experiment::Survey parseSurvey(const Value &jsonSurvey) {
    const auto &typesOrder = getNode(jsonSurvey, field20_TypesOrder);
    const auto &simpleQuestions = getNode(jsonSurvey, field21_SimpleQuestions);

    Experiment::Survey survey;
    for (auto t : parseIntArray(typesOrder)) {
        survey.order.push_back(static_cast<Experiment::Survey::QuestionType>(t));
    }
    survey.simpleQuestions = parseIntArray(simpleQuestions);
    return survey;
}
}
}
}
}