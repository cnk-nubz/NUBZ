#include <vector>

#include <boost/lexical_cast.hpp>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include "RawReportFactory.h"

// to json
namespace db {
namespace factory {
namespace json {

rapidjson::Value createHistory(const std::vector<db::RawReport::Event> &events,
                               rapidjson::Document::AllocatorType &allocator);
rapidjson::Value createEvent(const db::RawReport::Event &event,
                             rapidjson::Document::AllocatorType &allocator);
rapidjson::Value createActions(const std::vector<std::int32_t> &actions,
                               rapidjson::Document::AllocatorType &allocator);
rapidjson::GenericStringRef<char> toStupidStringAdapter(const std::string &str);

std::string RawReportFactory::createJson(const db::RawReport &report) {
    rapidjson::Document document;
    rapidjson::Value json;
    json.SetObject();

    auto &allocator = document.GetAllocator();
    json.AddMember(toStupidStringAdapter(db::info::reports::field0History),
                   createHistory(report.history, allocator),
                   allocator);

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    json.Accept(writer);
    return std::string(buffer.GetString());
}

rapidjson::Value createHistory(const std::vector<db::RawReport::Event> &events,
                               rapidjson::Document::AllocatorType &allocator) {
    rapidjson::Value json;
    json.SetArray();

    for (const auto &event : events) {
        json.PushBack(createEvent(event, allocator), allocator);
    }

    return json;
}

rapidjson::Value createEvent(const db::RawReport::Event &event,
                             rapidjson::Document::AllocatorType &allocator) {
    using namespace db::info::reports;
    rapidjson::Value json;
    json.SetObject();

    if (event.exhibitId) {
        json.AddMember(toStupidStringAdapter(field1ExhibitId),
                       rapidjson::Value(event.exhibitId.value()),
                       allocator);
    }

    json.AddMember(toStupidStringAdapter(field1DurationInSecs),
                   rapidjson::Value(event.durationInSecs),
                   allocator);
    json.AddMember(
        toStupidStringAdapter(field1Actions), createActions(event.actions, allocator), allocator);

    return json;
}

rapidjson::Value createActions(const std::vector<std::int32_t> &actions,
                               rapidjson::Document::AllocatorType &allocator) {
    rapidjson::Value json;
    json.SetArray();

    for (auto action : actions) {
        json.PushBack(rapidjson::Value(action), allocator);
    }

    return json;
}

rapidjson::GenericStringRef<char> toStupidStringAdapter(const std::string &str) {
    return rapidjson::GenericStringRef<char>(str.c_str());
}
}
}
}

// from json
namespace db {
namespace factory {
namespace json {

const rapidjson::Value &getNode(const rapidjson::Document &root, const std::string &path);
const rapidjson::Value &getNode(const rapidjson::Value &root, const std::string &path);

std::vector<RawReport::Event> parseHistory(const rapidjson::Value &jsonHistory);
RawReport::Event parseEvent(const rapidjson::Value &jsonEvent);
std::vector<std::int32_t> parseActions(const rapidjson::Value &jsonActions);

const std::vector<std::string> &RawReportFactory::fieldsOrder() noexcept {
    using namespace db::info::reports;
    static const std::vector<std::string> order = {colId, colDocument};
    return order;
}

RawReport RawReportFactory::create(const std::vector<boost::optional<std::string>> &raw) noexcept {
    using namespace db::info::reports;

    assert(raw.size() == fieldsOrder().size());
    assert(raw[0] && raw[1]);

    RawReport report;
    assert(boost::conversion::try_lexical_convert(raw[0].value(), report.ID));

    rapidjson::Document json;
    json.Parse(raw[1].value().c_str());

    const auto &history = getNode(json, field0History);
    report.history = parseHistory(history);

    return report;
}

std::vector<RawReport::Event> parseHistory(const rapidjson::Value &jsonHistory) {
    assert(jsonHistory.IsArray());

    std::vector<RawReport::Event> history;
    for (rapidjson::SizeType i = 0; i < jsonHistory.Size(); i++) {
        history.push_back(parseEvent(jsonHistory[i]));
    }
    return history;
}

RawReport::Event parseEvent(const rapidjson::Value &jsonEvent) {
    using namespace db::info::reports;

    assert(jsonEvent.IsObject());

    const auto &duration = getNode(jsonEvent, field1DurationInSecs);
    const auto &actions = getNode(jsonEvent, field1Actions);
    assert(duration.IsInt());

    RawReport::Event event;
    event.durationInSecs = duration.GetInt();
    event.actions = parseActions(actions);

    if (jsonEvent.HasMember(field1ExhibitId.c_str())) {
        const auto &exhibitId = getNode(jsonEvent, field1ExhibitId);
        assert(exhibitId.IsInt());
        event.exhibitId = exhibitId.GetInt();
    }

    return event;
}

std::vector<std::int32_t> parseActions(const rapidjson::Value &jsonActions) {
    assert(jsonActions.IsArray());

    std::vector<std::int32_t> actions;
    for (rapidjson::SizeType i = 0; i < jsonActions.Size(); i++) {
        assert(jsonActions[i].IsInt());
        actions.push_back(jsonActions[i].GetInt());
    }

    return actions;
}

const rapidjson::Value &getNode(const rapidjson::Document &root, const std::string &path) {
    assert(root.HasMember(path.c_str()));
    return root[path.c_str()];
}

const rapidjson::Value &getNode(const rapidjson::Value &root, const std::string &path) {
    assert(root.HasMember(path.c_str()));
    return root[path.c_str()];
}
}
}
}