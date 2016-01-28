#include <vector>

#include <boost/lexical_cast.hpp>

#include "RawReportFactory.h"
#include "commons.h"

// to json
namespace db {
namespace factory {
namespace json {

using namespace info::reports;
using namespace rapidjson;

namespace {
Value createEvent(const RawReport::Event &event, Document::AllocatorType &allocator);
Value createSurvey(const RawReport::Survey &survey, Document::AllocatorType &allocator);
Value createSimpleQuestionAnswer(const RawReport::Survey::SimpleQuestionAnswer &q,
                                 Document::AllocatorType &allocator);
}

std::string RawReportFactory::createJson(const RawReport &report) {
    static const auto keyHistory = toStupidStringAdapter(info::reports::field0_History);
    static const auto keySurveyBefore = toStupidStringAdapter(info::reports::field1_SurveyBefore);
    static const auto keySurveyAfter = toStupidStringAdapter(info::reports::field1_SurveyAfter);

    Document document;
    auto &allocator = document.GetAllocator();

    auto jsonHistory = createArray(report.history, allocator, createEvent);
    auto jsonSurveyBefore = createSurvey(report.surveyBefore, allocator);
    auto jsonSurveyAfter = createSurvey(report.surveyAfter, allocator);

    Value json(kObjectType);
    json.AddMember(keyHistory, jsonHistory, allocator);
    json.AddMember(keySurveyBefore, jsonSurveyBefore, allocator);
    json.AddMember(keySurveyAfter, jsonSurveyAfter, allocator);
    return jsonToString(json);
}

namespace {
Value createEvent(const RawReport::Event &event, Document::AllocatorType &allocator) {
    static const auto keyExhibitId = toStupidStringAdapter(field00_ExhibitId);
    static const auto keyDuration = toStupidStringAdapter(field01_DurationInSecs);
    static const auto keyActions = toStupidStringAdapter(field02_Actions);

    auto jsonDuration = Value(event.durationInSecs);
    auto jsonActions = createTrivialArray(event.actions, allocator);

    Value json(kObjectType);
    if (event.exhibitId) {
        json.AddMember(keyExhibitId, Value(event.exhibitId.value()), allocator);
    }
    json.AddMember(keyDuration, jsonDuration, allocator);
    json.AddMember(keyActions, jsonActions, allocator);
    return json;
}

Value createSurvey(const RawReport::Survey &survey, Document::AllocatorType &allocator) {
    static const auto keySimpleQuestions = toStupidStringAdapter(field10_SimpleQuestions);

    auto jsonSimpleQuestions =
        createArray(survey.simpleQuestions, allocator, createSimpleQuestionAnswer);

    Value json(kObjectType);
    json.AddMember(keySimpleQuestions, jsonSimpleQuestions, allocator);
    return json;
}

Value createSimpleQuestionAnswer(const RawReport::Survey::SimpleQuestionAnswer &ans,
                                 Document::AllocatorType &allocator) {
    static const auto keyAnswer = toStupidStringAdapter(field100_Answer);

    Value json(kObjectType);
    if (ans.answer) {
        json.AddMember(keyAnswer, toStupidStringAdapter(ans.answer.value()), allocator);
    }
    return json;
}
}
}
}
}

// from json
namespace db {
namespace factory {
namespace json {

namespace {
RawReport::Event parseEvent(const Value &jsonEvent);
RawReport::Survey parseSurvey(const Value &jsonSurvey);
RawReport::Survey::SimpleQuestionAnswer parseSimpleQuestionAnswer(const Value &jsonSimpleQ);
}

const std::vector<std::string> &RawReportFactory::fieldsOrder() noexcept {
    static const std::vector<std::string> order = {colId, colExperimentId, colDocument};
    return order;
}

RawReport RawReportFactory::create(const std::vector<boost::optional<std::string>> &raw) noexcept {
    assert(raw.size() == fieldsOrder().size());
    assert(std::all_of(raw.begin(), raw.end(), [](const auto &e) { return (bool)e; }));

    RawReport report;
    assert(boost::conversion::try_lexical_convert(raw[0].value(), report.ID));
    assert(boost::conversion::try_lexical_convert(raw[1].value(), report.experimentID));
    Document json = parseJson(raw[2].value());

    const auto &jsonHistory = getNode(json, field0_History);
    const auto &jsonBeforeSurvey = getNode(json, field1_SurveyBefore);
    const auto &jsonAfterSurvey = getNode(json, field1_SurveyAfter);

    report.history = parseArray(jsonHistory, parseEvent);
    report.surveyBefore = parseSurvey(jsonBeforeSurvey);
    report.surveyAfter = parseSurvey(jsonAfterSurvey);

    return report;
}

namespace {
RawReport::Event parseEvent(const Value &jsonEvent) {
    assert(jsonEvent.IsObject());

    RawReport::Event event;
    event.durationInSecs = parseInt(jsonEvent, field01_DurationInSecs);
    event.actions = parseIntArray(getNode(jsonEvent, field02_Actions));
    event.exhibitId = parseOptInt(jsonEvent, field00_ExhibitId);
    return event;
}

RawReport::Survey parseSurvey(const Value &jsonSurvey) {
    assert(jsonSurvey.IsObject());

    const auto &simpleQuestions = getNode(jsonSurvey, field10_SimpleQuestions);

    RawReport::Survey survey;
    survey.simpleQuestions = parseArray(simpleQuestions, parseSimpleQuestionAnswer);
    return survey;
}

RawReport::Survey::SimpleQuestionAnswer parseSimpleQuestionAnswer(const Value &jsonSimpleQ) {
    assert(jsonSimpleQ.IsObject());

    RawReport::Survey::SimpleQuestionAnswer sQ;
    sQ.answer = parseOptString(jsonSimpleQ, field100_Answer);
    return sQ;
}
}
}
}
}