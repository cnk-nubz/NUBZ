#ifndef IO__EXPERIMENT_DATA__H
#define IO__EXPERIMENT_DATA__H

#include <cstdint>
#include <vector>

#include <communication/structs_types.h>
#include <server/io/Action.h>

namespace server {
namespace io {
namespace output {

class ExperimentData {
public:
    communication::ExperimentData toThrift() const;

    std::vector<io::Action> exhibitActions;
    std::vector<io::Action> breakActions;

private:
    std::vector<communication::Action> toThriftActions(
        const std::vector<io::Action> &ioActions) const;
};
}
}
}

#endif