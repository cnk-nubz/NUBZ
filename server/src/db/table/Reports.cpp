#include <utils/fp_algorithm.h>

#include <db/json/json.h>

#include "Reports.h"

/*
 * columns:
 * - id
 * - name
 * - state
 * - startDate
 * - finishDate
 * - content (json)
 *
 * content = {events, surveyBefore, surveyAfter}
 * events = [event, ...]
 * event = {optional(exhibitId), durationInSecs, actions}
 * exhibitId, durationInSecs = int
 * actions = [int, ...]
 * survey = {simpleQuestionsAnswers, multipleChoiceQuestionsAnswers, sortQuestionsAnswers}
 * anyAnswers = [answer, ...]
 * simpleQuestionsAnswers = string
 * multipleChoiceQuestionsAnswer, sortQuestionsAnswer = [int, ...]
 */

namespace db {
namespace table {

const std::string Reports::tableName = "reports";

const std::string Reports::FieldID::columnName = "id";
const std::string Reports::FieldExperimentID::columnName = "experiment_id";
const std::string Reports::FieldContent::columnName = "content";

namespace {
struct RootKeys {
    static const char *history;
    static const char *surveyBefore;
    static const char *surveyAfter;
    struct EventKeys {
        static const char *exhibitId;
        static const char *duration;
        static const char *actions;
    };
    struct SurveyAnsKeys {
        static const char *simpleQ;
        static const char *multiQ;
        static const char *sortQ;
        struct AnsKeys {
            static const char *answer;
        };
    };
};

const char *RootKeys::history = "history";
const char *RootKeys::surveyBefore = "surveyBefore";
const char *RootKeys::surveyAfter = "surveyAfter";

const char *RootKeys::EventKeys::exhibitId = "exhibitId";
const char *RootKeys::EventKeys::duration = "secs";
const char *RootKeys::EventKeys::actions = "actions";

const char *RootKeys::SurveyAnsKeys::simpleQ = "simple";
const char *RootKeys::SurveyAnsKeys::multiQ = "multi";
const char *RootKeys::SurveyAnsKeys::sortQ = "sort";

const char *RootKeys::SurveyAnsKeys::AnsKeys::answer = "ans";
}

using namespace db::json;

namespace {
Reports::ContentData::Event parseEvent(const rapidjson::Value &json);
Reports::ContentData::SurveyAns parseSurveyAns(const rapidjson::Value &json);
Reports::ContentData::SurveyAns::SimpleQAnswer parseSimpleAns(const rapidjson::Value &json);
Reports::ContentData::SurveyAns::MultiChoiceQAnswer parseMultiAns(const rapidjson::Value &json);
Reports::ContentData::SurveyAns::SortQAnswer parseSortAns(const rapidjson::Value &json);
}

Reports::ContentData::ContentData(const std::string &jsonStr) {
    using Keys = RootKeys;
    auto json = parseJson(jsonStr);
    history = parseArray(getNode(json, Keys::history), parseEvent);
    surveyBefore = parseSurveyAns(getNode(json, Keys::surveyBefore));
    surveyAfter = parseSurveyAns(getNode(json, Keys::surveyAfter));
}

namespace {
Reports::ContentData::Event parseEvent(const rapidjson::Value &json) {
    using Keys = RootKeys::EventKeys;
    auto event = Reports::ContentData::Event{};
    event.exhibitID = parseOpt(json, Keys::exhibitId, parseInt);
    event.durationInSecs = parseInt(getNode(json, Keys::duration));
    event.actions = parseIntArray(getNode(json, Keys::actions));
    return event;
}

Reports::ContentData::SurveyAns parseSurveyAns(const rapidjson::Value &json) {
    using Keys = RootKeys::SurveyAnsKeys;
    auto surveyAns = Reports::ContentData::SurveyAns{};
    surveyAns.simpleQAnswers = parseArray(getNode(json, Keys::simpleQ), parseSimpleAns);
    surveyAns.multiChoiceQAnswers = parseArray(getNode(json, Keys::multiQ), parseMultiAns);
    surveyAns.sortQAnswers = parseArray(getNode(json, Keys::sortQ), parseSortAns);
    return surveyAns;
}

Reports::ContentData::SurveyAns::SimpleQAnswer parseSimpleAns(const rapidjson::Value &json) {
    using Keys = RootKeys::SurveyAnsKeys::AnsKeys;
    return parseString(getNode(json, Keys::answer));
}

Reports::ContentData::SurveyAns::MultiChoiceQAnswer parseMultiAns(const rapidjson::Value &json) {
    using Keys = RootKeys::SurveyAnsKeys::AnsKeys;
    return parseIntArray(getNode(json, Keys::answer));
}

Reports::ContentData::SurveyAns::SortQAnswer parseSortAns(const rapidjson::Value &json) {
    using Keys = RootKeys::SurveyAnsKeys::AnsKeys;
    return parseIntArray(getNode(json, Keys::answer));
}
}

namespace {
rapidjson::Value createEvent(rapidjson::Document::AllocatorType &allocator,
                             const Reports::ContentData::Event &event);
rapidjson::Value createSurveyAns(rapidjson::Document::AllocatorType &allocator,
                                 const Reports::ContentData::SurveyAns &sAns);
rapidjson::Value createSimpleAns(rapidjson::Document::AllocatorType &allocator,
                                 const Reports::ContentData::SurveyAns::SimpleQAnswer &ans);
rapidjson::Value createMultiAns(rapidjson::Document::AllocatorType &allocator,
                                const Reports::ContentData::SurveyAns::MultiChoiceQAnswer &ans);
rapidjson::Value createSortAns(rapidjson::Document::AllocatorType &allocator,
                               const Reports::ContentData::SurveyAns::SortQAnswer &ans);
}

Reports::ContentData::operator std::string() const {
    using Keys = RootKeys;

    auto document = rapidjson::Document{};
    auto &allocator = document.GetAllocator();

    auto json = createDictionary(
        allocator,
        std::make_pair(Keys::history, createArray(allocator, history, createEvent)),
        std::make_pair(Keys::surveyBefore, createSurveyAns(allocator, surveyBefore)),
        std::make_pair(Keys::surveyAfter, createSurveyAns(allocator, surveyAfter)));
    return jsonToString(json);
}

namespace {
rapidjson::Value createEvent(rapidjson::Document::AllocatorType &allocator,
                             const Reports::ContentData::Event &event) {
    using Keys = RootKeys::EventKeys;
    if (event.exhibitID) {
        return createDictionary(
            allocator,
            std::make_pair(Keys::exhibitId, createInt(event.exhibitID.value())),
            std::make_pair(Keys::duration, createInt(event.durationInSecs)),
            std::make_pair(Keys::actions, createIntArray(allocator, event.actions)));
    } else {
        return createDictionary(
            allocator,
            std::make_pair(Keys::duration, createInt(event.durationInSecs)),
            std::make_pair(Keys::actions, createIntArray(allocator, event.actions)));
    }
}

rapidjson::Value createSurveyAns(rapidjson::Document::AllocatorType &allocator,
                                 const Reports::ContentData::SurveyAns &sAns) {
    using Keys = RootKeys::SurveyAnsKeys;

    auto simple = createArray(allocator, sAns.simpleQAnswers, createSimpleAns);
    auto multi = createArray(allocator, sAns.multiChoiceQAnswers, createMultiAns);
    auto sort = createArray(allocator, sAns.sortQAnswers, createSortAns);

    return createDictionary(allocator,
                            std::make_pair(Keys::simpleQ, std::move(simple)),
                            std::make_pair(Keys::multiQ, std::move(multi)),
                            std::make_pair(Keys::sortQ, std::move(sort)));
}

rapidjson::Value createSimpleAns(rapidjson::Document::AllocatorType &allocator,
                                 const Reports::ContentData::SurveyAns::SimpleQAnswer &ans) {
    using Keys = RootKeys::SurveyAnsKeys::AnsKeys;
    return createDictionary(allocator, std::make_pair(Keys::answer, createString(ans)));
}

rapidjson::Value createMultiAns(rapidjson::Document::AllocatorType &allocator,
                                const Reports::ContentData::SurveyAns::MultiChoiceQAnswer &ans) {
    using Keys = RootKeys::SurveyAnsKeys::AnsKeys;
    return createDictionary(allocator,
                            std::make_pair(Keys::answer, createIntArray(allocator, ans)));
}

rapidjson::Value createSortAns(rapidjson::Document::AllocatorType &allocator,
                               const Reports::ContentData::SurveyAns::SortQAnswer &ans) {
    using Keys = RootKeys::SurveyAnsKeys::AnsKeys;
    return createDictionary(allocator,
                            std::make_pair(Keys::answer, createIntArray(allocator, ans)));
}
}
}
}
