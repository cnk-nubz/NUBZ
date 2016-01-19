#include "IncrementCounter.h"
#include "GetCounter.h"
#include "SetCounter.h"

namespace db {
namespace cmd {

IncrementCounter IncrementCounter::exhibitVersion() {
    return IncrementCounter(db::info::counters::element_type::exhibits);
}

IncrementCounter IncrementCounter::mapVersion() {
    return IncrementCounter(db::info::counters::element_type::map_images);
}

IncrementCounter IncrementCounter::reportId() {
    return IncrementCounter(db::info::counters::element_type::reports);
}

IncrementCounter::IncrementCounter(db::info::counters::element_type elementType)
    : elementType(elementType) {
}

std::int32_t IncrementCounter::operator()(db::DatabaseSession &session) {
    newVal = GetCounter{elementType}(session) + 1;
    SetCounter{elementType, newVal}(session);
    return newVal;
}

std::int32_t IncrementCounter::getNewVal() const {
    return newVal;
}
}
}