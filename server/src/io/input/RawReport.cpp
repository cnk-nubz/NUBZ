#include "RawReport.h"

namespace io {
    namespace input {
        RawReport::RawReport(const communication::RawReport &thrift) {
            reportId = thrift.reportId;
            for (const auto &event : thrift.history) {
                history.push_back(Event(event));
            }
        }
        
        db::RawReport RawReport::toDb() const {
            db::RawReport res;
            res.ID = reportId;
            for (const auto &event : history) {
                res.history.push_back(event.toDb());
            }
            return res;
        }
        
        RawReport::Event::Event(const communication::RawReportEvent &thrift) : breakEvent(true) {
            if (thrift.__isset.exhibitId) {
                breakEvent = false;
                exhibitId = thrift.exhibitId;
            }
            durationInSecs = thrift.durationInSecs;
            actions = thrift.actions;
        }
        
        db::RawReport::Event RawReport::Event::toDb() const {
            db::RawReport::Event res;
            if (!breakEvent) {
                res.exhibitId = exhibitId;
            }
            res.durationInSecs = durationInSecs;
            res.actions = actions;
            return res;
        }
        
        bool RawReport::Event::isBreak() const {
            return breakEvent;
        }
        
        std::int32_t RawReport::Event::getExhibitId() const {
            assert(breakEvent == false);
            return exhibitId;
        }
    }
}