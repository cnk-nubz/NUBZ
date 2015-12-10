#include <vector>

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include "JsonReportFactory.h"

namespace db {
    namespace factory {
        rapidjson::Value createHistory(const std::vector<db::RawReport::Event> &events,
                                       rapidjson::Document::AllocatorType &allocator);
        rapidjson::Value createEvent(const db::RawReport::Event &event,
                                     rapidjson::Document::AllocatorType &allocator);
        rapidjson::Value createActions(const std::vector<std::int32_t> &actions,
                                       rapidjson::Document::AllocatorType &allocator);
        rapidjson::GenericStringRef<char> toStupidStringAdapter(const std::string &str);

        std::string JsonReportFactory::createJson(const db::RawReport &report) {
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
            json.AddMember(toStupidStringAdapter(field1Actions),
                           createActions(event.actions, allocator),
                           allocator);

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