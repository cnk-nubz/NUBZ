#ifndef DB_CMD__GET_VERSION__H
#define DB_CMD__GET_VERSION__H

#include "DatabaseCommand.h"
#include "db/struct/MapImage.h"

namespace db {
    namespace cmd {
        class GetVersion : public DatabaseCommand {
        public:
            enum class ElementType { MapImage };

            GetVersion(ElementType elementType);
            virtual ~GetVersion() = default;

            GetVersion(const GetVersion &) = delete;
            GetVersion(GetVersion &&) = default;
            GetVersion &operator=(const GetVersion &) = delete;
            GetVersion &operator=(GetVersion &&) = default;

            virtual void perform(DatabaseSession &session);
            std::int32_t getResult() const;

        private:
            ElementType elementType;
            std::int32_t result;

            std::string createQuery() const;
            std::string elementTypeName() const;
        };
    }
}

#endif