#include "ActionFactory.h"

namespace db {
    namespace factory {
        ActionFactory::Product ActionFactory::create(
            const std::vector<boost::optional<std::string>> &raw) noexcept {
            Action res;
            assert(raw.size() == fieldsOrder().size());
            assert(boost::conversion::try_lexical_convert(raw[0].value(), res.ID));
            res.text = raw[1].value();
            assert(boost::conversion::try_lexical_convert(raw[2].value(), res.duringBreak));
            assert(boost::conversion::try_lexical_convert(raw[3].value(), res.inCurrentExperiment));
            return res;
        }

        const std::vector<std::string> &ActionFactory::fieldsOrder() noexcept {
            using namespace db::info::actions;
            static const std::vector<std::string> order = {colId, colText, colDuringBreak,
                                                           colInCurrentExperiment};
            return order;
        }
    }
}