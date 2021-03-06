#ifndef DB_SQL__WHERE__H
#define DB_SQL__WHERE__H

#include <boost/optional.hpp>

#include <utils/type_traits.h>

#include "Condition.h"

namespace db {
namespace sql {

template <class Subclass, class... AvailableFields>
class Where {
public:
    template <class... UsedFields>
    Subclass &where(const detail::Condition<UsedFields...> &cond) {
        static_assert(::utils::types::all_of<
                          ::utils::types::find_type<UsedFields, AvailableFields...>...>::value,
                      "one of used fields is not available in this sql query");
        if (this->cond) {
            this->cond = (detail::Condition<>{std::move(this->cond.value())} && cond).str();
        } else {
            this->cond = cond.str();
        }
        return *static_cast<Subclass *>(this);
    }

protected:
    std::string whereStmt() const {
        return cond ? "WHERE " + cond.value() : "";
    }

private:
    boost::optional<std::string> cond;
};
}
}

#endif