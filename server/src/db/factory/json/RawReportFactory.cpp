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
Value createSimpleQuestionAnswer(const RawReport::Survey::SimpleQuestionAnswer &ans,
                                 Document::AllocatorType &allocator);
Value createMultipleChoiceQuestionAnswer(const RawReport::Survey::MultipleChoiceQuestionAnswer &ans,
                                         Document::AllocatorType &allocator);
Value createSortQuestionAnswer(const RawReport::Survey::SortQuestionAnswer &ans,
                               Document::AllocatorType &allocator);
}

std::string RawReportFactory::createJson(const RawReport &report) {
    static const auto keyHistory = toStrAdapter(info::reports::field0_History);
    static const auto keySurveyBefore = toStrAdapter(info::reports::field1_SurveyBefore);
    static const auto keySurveyAfter = toStrAdapter(info::reports::field1_SurveyAfter);

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
    static const auto keyExhibitId = toStrAdapter(field00_ExhibitId);
    static const auto keyDuration = toStrAdapter(field01_DurationInSecs);
    static const auto keyActions = toStrAdapter(field02_Actions);

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
    static const auto keySimpleQuestions = toStrAdapter(field10_SimpleQuestions);
    static const auto keyMultipleChoiceQuestions = toStrAdapter(field11_MultipleChoiceQuestions);
    static const auto keySortQuetions = toStrAdapter(field12_SortQuestions);

    auto jsonSimpleQuestions =
        createArray(survey.simpleQuestions, allocator, createSimpleQuestionAnswer);
    auto jsonMultipleChoiceQuestions =
        createArray(survey.multipleChoiceQuestions, allocator, createMultipleChoiceQuestionAnswer);
    auto jsonSortQuestions = createArray(survey.sortQuestions, allocator, createSortQuestionAnswer);

    Value json(kObjectType);
    json.AddMember(keySimpleQuestions, jsonSimpleQuestions, allocator);
    json.AddMember(keyMultipleChoiceQuestions, jsonMultipleChoiceQuestions, allocator);
    json.AddMember(keySortQuetions, jsonSortQuestions, allocator);
    return json;
}

Value createSimpleQuestionAnswer(const RawReport::Survey::SimpleQuestionAnswer &ans,
                                 Document::AllocatorType &allocator) {
    static const auto keyAnswer = toStrAdapter(field100_Answer);

    Value json(kObjectType);
    if (ans.answer) {
        json.AddMember(keyAnswer, toStrAdapter(ans.answer.value()), allocator);
    }
    return json;
}

Value createMultipleChoiceQuestionAnswer(const RawReport::Survey::MultipleChoiceQuestionAnswer &ans,
                                         Document::AllocatorType &allocator) {
    static const auto keyAnswer = toStrAdapter(field110_Answer);

    Value json(kObjectType);
    if (ans.choosenOptions) {
        auto jsonChoosenOptions = createTrivialArray(ans.choosenOptions.value(), allocator);
        json.AddMember(keyAnswer, jsonChoosenOptions, allocator);
    }
    return json;
}

Value createSortQuestionAnswer(const RawReport::Survey::SortQuestionAnswer &ans,
                               Document::AllocatorType &allocator) {
    static const auto keyAnswer = toStrAdapter(field120_Answer);

    Value json(kObjectType);
    if (ans.choosenOrder) {
        auto jsonChoosenOrder = createTrivialArray(ans.choosenOrder.value(), allocator);
        json.AddMember(keyAnswer, jsonChoosenOrder, allocator);
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
RawReport::Event parseEvent(const Value &json);
RawReport::Survey parseSurvey(const Value &json);
RawReport::Survey::SimpleQuestionAnswer parseSimpleQuestionAnswer(const Value &json);
RawReport::Survey::MultipleChoiceQuestionAnswer parseMultipleChoiceQuestionAnswer(
    const Value &json);
RawReport::Survey::SortQuestionAnswer parseSortQuestionAnswer(const Value &json);
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

    report.history = parseArray(parseEvent, getNode(json, field0_History));
    report.surveyBefore = parse(parseSurvey, json, field1_SurveyBefore);
    report.surveyAfter = parse(parseSurvey, json, field1_SurveyAfter);

    return report;
}

namespace {
RawReport::Event parseEvent(const Value &json) {
    assert(json.IsObject());

    RawReport::Event event;
    event.durationInSecs = parse(parseInt, json, field01_DurationInSecs);
    event.actions = parse(parseIntArray, json, field02_Actions);
    event.exhibitId = parseOpt(parseInt, json, field00_ExhibitId);
    return event;
}

RawReport::Survey parseSurvey(const Value &json) {
    assert(json.IsObject());

    const auto &jsonSimpleQuestions = getNode(json, field10_SimpleQuestions);
    const auto &jsonMultipleChoiceQuestions = getNode(json, field11_MultipleChoiceQuestions);
    const auto &jsonSortQuestions = getNode(json, field12_SortQuestions);

    RawReport::Survey survey;
    survey.simpleQuestions = parseArray(parseSimpleQuestionAnswer, jsonSimpleQuestions);
    survey.multipleChoiceQuestions =
        parseArray(parseMultipleChoiceQuestionAnswer, jsonMultipleChoiceQuestions);
    survey.sortQuestions = parseArray(parseSortQuestionAnswer, jsonSortQuestions);
    return survey;
}

RawReport::Survey::SimpleQuestionAnswer parseSimpleQuestionAnswer(const Value &json) {
    assert(json.IsObject());

    RawReport::Survey::SimpleQuestionAnswer ans;
    ans.answer = parseOpt(parseString, json, field100_Answer);
    return ans;
}

RawReport::Survey::MultipleChoiceQuestionAnswer parseMultipleChoiceQuestionAnswer(
    const Value &json) {
    assert(json.IsObject());

    RawReport::Survey::MultipleChoiceQuestionAnswer ans;
    ans.choosenOptions = parseOpt(parseIntArray, json, field110_Answer);
    return ans;
}

RawReport::Survey::SortQuestionAnswer parseSortQuestionAnswer(const Value &json) {
    assert(json.IsObject());

    RawReport::Survey::SortQuestionAnswer ans;
    ans.choosenOrder = parseOpt(parseIntArray, json, field120_Answer);
    return ans;
}
}
}
}
}