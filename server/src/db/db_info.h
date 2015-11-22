#ifndef DB__DB_INFO__H
#define DB__DB_INFO__H

namespace db {
    namespace info {
        namespace map_images {
            static const std::string tableName = "map_images";

            static const std::string colFilename = "filename";
            static const std::string colVersion = "version";
            static const std::string colLevel = "level";
        };

        namespace versions {
            static const std::string tableName = "versions";

            static const std::string colElement = "element";
            static const std::string colVersion = "version";

            enum class element_type {
                map_images,
            };

            inline static std::string colElementType(element_type t) {
                switch (t) {
                    case element_type::map_images:
                        return "map_images";
                }
            }
        }
    }
}

#endif