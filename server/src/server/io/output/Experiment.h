#ifndef IO__EXPERIMENT__H
#define IO__EXPERIMENT__H

#include <cstdint>
#include <vector>

#include <communication/structs_types.h>
#include <server/io/Action.h>

namespace server {
namespace io {
namespace output {

class Experiment {
public:
    communication::Experiment toThrift() const;

    std::int32_t experimentID;
    std::string name;
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