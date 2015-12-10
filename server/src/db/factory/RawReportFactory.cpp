#include <rapidjson/document.h>

#include "RawReportFactory.h"

namespace db {
    namespace factory {
        const rapidjson::Value &getNode(const rapidjson::Document &root, const std::string &path);
        const rapidjson::Value &getNode(const rapidjson::Value &root, const std::string &path);
        std::vector<RawReport::Event> parseHistory(const rapidjson::Value &jsonHistory);
        RawReport::Event parseEvent(const rapidjson::Value &jsonEvent);
        std::vector<std::int32_t> parseActions(const rapidjson::Value &jsonActions);

        RawReport RawReportFactory::create(
            const std::vector<boost::optional<std::string>> &raw) noexcept {
            using namespace db::info::reports;

            RawReport report;

            assert(boost::conversion::try_lexical_convert(raw[0].value(), report.ID));
            assert(raw[1]);

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

            RawReport::Event event;

            assert(duration.IsInt());
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