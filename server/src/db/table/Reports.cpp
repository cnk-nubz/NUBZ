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
 * anyAnswers = [optional(answer), ...]
 * simpleQuestionsAnswers = string
 * multipleChoiceQuestionsAnswer, sortQuestionsAnswer = [int, ...]
 */

namespace db {
namespace table {

const std::string Reports::tableName = "reports";
const std::string Reports::ColumnId::name = "id";
const std::string Reports::ColumnExperimentId::name = "experiment_id";
const std::string Reports::ColumnContent::name = "content";

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

const Reports::ColumnId Reports::colId{};
const Reports::ColumnExperimentId Reports::colExperimentId{};
const Reports::ColumnContent Reports::colContent{};

namespace {
// returns {history, surveyBefore, surveyAfter>
std::tuple<std::vector<Reports::Row::Event>, Reports::Row::SurveyAns, Reports::Row::SurveyAns>
parseContent(const std::string &jsonStr);

Reports::Row::Event parseEvent(const rapidjson::Value &json);

Reports::Row::SurveyAns parseSurveyAns(const rapidjson::Value &json);
Reports::Row::SurveyAns::SimpleQAnswer parseSimpleAns(const rapidjson::Value &json);
Reports::Row::SurveyAns::MultiChoiceQAnswer parseMultiAns(const rapidjson::Value &json);
Reports::Row::SurveyAns::SortQAnswer parseSortAns(const rapidjson::Value &json);
}

Reports::Row Reports::RowFactory::fromDB(const DBOut &dbOut) {
    auto res = Reports::Row{};
    res.ID = std::get<ValueId>(dbOut).value;
    res.experimentID = std::get<ValueExperimentId>(dbOut).value;
    std::tie(res.history, res.surveyBefore, res.surveyAfter) =
        parseContent(std::get<ValueContent>(dbOut).value);
    return res;
}

namespace {
using namespace db::json;

std::tuple<std::vector<Reports::Row::Event>, Reports::Row::SurveyAns, Reports::Row::SurveyAns>
parseContent(const std::string &jsonStr) {
    using Keys = RootKeys;
    auto json = parseJson(jsonStr);
    return std::make_tuple(parseArray(getNode(json, Keys::history), parseEvent),
                           parseSurveyAns(getNode(json, Keys::surveyBefore)),
                           parseSurveyAns(getNode(json, Keys::surveyAfter)));
}

Reports::Row::Event parseEvent(const rapidjson::Value &json) {
    using Keys = RootKeys::EventKeys;
    auto event = Reports::Row::Event{};
    event.exhibitID = parseOpt(json, Keys::exhibitId, parseInt);
    event.durationInSecs = parseInt(getNode(json, Keys::duration));
    event.actions = parseIntArray(getNode(json, Keys::actions));
    return event;
}

Reports::Row::SurveyAns parseSurveyAns(const rapidjson::Value &json) {
    using Keys = RootKeys::SurveyAnsKeys;
    auto surveyAns = Reports::Row::SurveyAns{};
    surveyAns.simpleQAnswers = parseArray(getNode(json, Keys::simpleQ), parseSimpleAns);
    surveyAns.multiChoiceQAnswers = parseArray(getNode(json, Keys::multiQ), parseMultiAns);
    surveyAns.sortQAnswers = parseArray(getNode(json, Keys::sortQ), parseSortAns);
    return surveyAns;
}

Reports::Row::SurveyAns::SimpleQAnswer parseSimpleAns(const rapidjson::Value &json) {
    using Keys = RootKeys::SurveyAnsKeys::AnsKeys;
    return parseOpt(json, Keys::answer, parseString);
}

Reports::Row::SurveyAns::MultiChoiceQAnswer parseMultiAns(const rapidjson::Value &json) {
    using Keys = RootKeys::SurveyAnsKeys::AnsKeys;
    return parseOpt(json, Keys::answer, parseIntArray);
}

Reports::Row::SurveyAns::SortQAnswer parseSortAns(const rapidjson::Value &json) {
    using Keys = RootKeys::SurveyAnsKeys::AnsKeys;
    return parseOpt(json, Keys::answer, parseIntArray);
}
}

namespace {
std::string createContent(const Reports::Row &row);

rapidjson::Value createEvent(rapidjson::Document::AllocatorType &allocator,
                             const Reports::Row::Event &event);

rapidjson::Value createSurveyAns(rapidjson::Document::AllocatorType &allocator,
                                 const Reports::Row::SurveyAns &sAns);
rapidjson::Value createSimpleAns(rapidjson::Document::AllocatorType &allocator,
                                 const Reports::Row::SurveyAns::SimpleQAnswer &ans);
rapidjson::Value createMultiAns(rapidjson::Document::AllocatorType &allocator,
                                const Reports::Row::SurveyAns::MultiChoiceQAnswer &ans);
rapidjson::Value createSortAns(rapidjson::Document::AllocatorType &allocator,
                               const Reports::Row::SurveyAns::SortQAnswer &ans);
}

Reports::RowFactory::DBIn Reports::RowFactory::toDB(const db::table::Reports::Row &row) {
    return std::make_tuple(
        ValueId{row.ID}, ValueExperimentId{row.experimentID}, ValueContent{createContent(row)});
}

namespace {
using namespace db::json;

std::string createContent(const Reports::Row &row) {
    using Keys = RootKeys;
    auto document = rapidjson::Document{};
    auto &allocator = document.GetAllocator();

    auto json = createDictionary(
        allocator,
        std::make_pair(Keys::history, createArray(allocator, row.history, createEvent)),
        std::make_pair(Keys::surveyBefore, createSurveyAns(allocator, row.surveyBefore)),
        std::make_pair(Keys::surveyAfter, createSurveyAns(allocator, row.surveyAfter)));
    return jsonToString(json);
}

rapidjson::Value createEvent(rapidjson::Document::AllocatorType &allocator,
                             const Reports::Row::Event &event) {
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
                                 const Reports::Row::SurveyAns &sAns) {
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
                                 const Reports::Row::SurveyAns::SimpleQAnswer &ans) {
    using Keys = RootKeys::SurveyAnsKeys::AnsKeys;
    if (ans) {
        return createDictionary(allocator, std::make_pair(Keys::answer, createString(ans.value())));
    } else {
        return {};
    }
}

rapidjson::Value createMultiAns(rapidjson::Document::AllocatorType &allocator,
                                const Reports::Row::SurveyAns::MultiChoiceQAnswer &ans) {
    using Keys = RootKeys::SurveyAnsKeys::AnsKeys;
    if (ans) {
        return createDictionary(
            allocator, std::make_pair(Keys::answer, createIntArray(allocator, ans.value())));
    } else {
        return {};
    }
}

rapidjson::Value createSortAns(rapidjson::Document::AllocatorType &allocator,
                               const Reports::Row::SurveyAns::SortQAnswer &ans) {
    using Keys = RootKeys::SurveyAnsKeys::AnsKeys;
    if (ans) {
        return createDictionary(
            allocator, std::make_pair(Keys::answer, createIntArray(allocator, ans.value())));
    } else {
        return {};
    }
}
}
}
}
