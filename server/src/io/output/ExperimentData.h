#ifndef IO__EXPERIMENT_DATA__H
#define IO__EXPERIMENT_DATA__H

#include <cstdint>

#include "communication/structs_types.h"
#include "io/Action.h"

namespace io {
    namespace output {
        class ExperimentData {
        public:
            ExperimentData() = default;
            ~ExperimentData() = default;

            ExperimentData(const ExperimentData &) = default;
            ExperimentData(ExperimentData &&) = default;
            ExperimentData &operator=(const ExperimentData &) = default;
            ExperimentData &operator=(ExperimentData &&) = default;

            communication::ExperimentData toThrift() const;

            std::vector<io::Action> exhibitActions;
            std::vector<io::Action> breakActions;

        private:
            std::vector<communication::Action> toThriftActions(
                const std::vector<io::Action> &ioActions) const;
        };
    }
}

#endif