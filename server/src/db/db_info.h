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

            namespace element_option {
                static const std::string map_image = "map_images";
            }
        }
    }
}

#endif