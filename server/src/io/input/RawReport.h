#ifndef IO__RAW_REPORT__H
#define IO__RAW_REPORT__H

#include <cstdint>
#include <iostream>

#include "communication/structs_types.h"
#include "db/struct/RawReport.h"

namespace io {
    namespace input {
        class RawReport {
        public:
            class Event {
            public:
                Event() = default;
                ~Event() = default;

                Event(const Event &) = default;
                Event(Event &&) = default;
                Event &operator=(const Event &) = default;
                Event &operator=(Event &&) = default;

                Event(const communication::RawReportEvent &thrift);

                db::RawReport::Event toDb() const;
                
                bool isBreak() const;
                std::int32_t getExhibitId() const;

                std::vector<std::int32_t> actions;
                std::int32_t durationInSecs;

            private:
                bool breakEvent;
                std::int32_t exhibitId;
            };

            RawReport() = default;
            ~RawReport() = default;

            RawReport(const RawReport &) = default;
            RawReport(RawReport &&) = default;
            RawReport &operator=(const RawReport &) = default;
            RawReport &operator=(RawReport &&) = default;

            RawReport(const communication::RawReport &thrift);

            db::RawReport toDb() const;

            std::int32_t reportId;
            std::vector<Event> history;
        };
    }
}

#endif