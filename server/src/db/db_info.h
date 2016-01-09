#ifndef DB__DB_INFO__H
#define DB__DB_INFO__H

#include <string>

namespace db {
    namespace info {
        namespace map_images {
            static const std::string tableName = "map_images";

            static const std::string colFilename = "filename";
            static const std::string colWidth = "width";
            static const std::string colHeight = "height";
            static const std::string colVersion = "version";
            static const std::string colFloor = "floor";
        };

        namespace map_tiles {
            static const std::string tableName = "map_tiles";

            static const std::string colFilename = "filename";
            static const std::string colFloor = "floor";
            static const std::string colZoomLevel = "zoom_level";
            static const std::string colRow = "row";
            static const std::string colColumn = "col";
        }

        namespace map_tiles_info {
            static const std::string tableName = "map_tiles_info";

            static const std::string colFloor = "floor";
            static const std::string colZoomLevel = "zoom_level";
            static const std::string colRowsCount = "rows_count";
            static const std::string colColumnsCount = "columns_count";
            static const std::string colImgWidth = "img_width";
            static const std::string colImgHeight = "img_height";
            static const std::string colTileSize = "tile_size";
        }

        namespace counters {
            static const std::string tableName = "counters";

            static const std::string colElement = "element";
            static const std::string colVersion = "counter";

            enum class element_type {
                map_images,
                exhibits,
                reports,
            };

            inline static std::string colElementType(element_type t) {
                switch (t) {
                    case element_type::map_images:
                        return "map_images_version";
                    case element_type::exhibits:
                        return "exhibits_version";
                    case element_type::reports:
                        return "reports_last_id";
                }
            }
        }

        namespace exhibits {
            static const std::string tableName = "exhibits";

            static const std::string colId = "id";
            static const std::string colName = "name";
            static const std::string colVersion = "version";
            static const std::string colMapFrameX = "map_frame_x";
            static const std::string colMapFrameY = "map_frame_y";
            static const std::string colMapFrameWidth = "map_frame_width";
            static const std::string colMapFrameHeight = "map_frame_height";
            static const std::string colMapLevel = "map_level";
        }

        namespace reports {
            static const std::string tableName = "reports";

            static const std::string colId = "id";
            static const std::string colDocument = "doc";

            // json
            // field[level][name]
            static const std::string field0History = "history";
            static const std::string field1ExhibitId = "exhibitId";
            static const std::string field1DurationInSecs = "durationInSecs";
            static const std::string field1Actions = "actions";
        }
        
        namespace actions {
            static const std::string tableName = "actions";
            
            static const std::string colId = "id";
            static const std::string colText = "text";
            static const std::string colDuringBreak = "during_break";
            static const std::string colInCurrentExperiment = "in_current_experiment";
        }
    }
}

#endif