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
}  // map_images

namespace map_tiles {
static const std::string tableName = "map_tiles";

static const std::string colFilename = "filename";
static const std::string colFloor = "floor";
static const std::string colZoomLevel = "zoom_level";
static const std::string colRow = "row";
static const std::string colColumn = "col";
}  // map_tiles

namespace map_tiles_info {
static const std::string tableName = "map_tiles_info";

static const std::string colFloor = "floor";
static const std::string colZoomLevel = "zoom_level";
static const std::string colRowsCount = "rows_count";
static const std::string colColumnsCount = "columns_count";
static const std::string colImgWidth = "img_width";
static const std::string colImgHeight = "img_height";
static const std::string colTileSize = "tile_size";
}  // map_tiles_info

namespace counters {
static const std::string tableName = "counters";

static const std::string colElement = "element";
static const std::string colValue = "counter";

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
}  // counters

namespace exhibits {
static const std::string tableName = "exhibits";

static const std::string colId = "id";
static const std::string colName = "name";
static const std::string colVersion = "version";
static const std::string colMapFrameX = "map_frame_x";
static const std::string colMapFrameY = "map_frame_y";
static const std::string colMapFrameWidth = "map_frame_width";
static const std::string colMapFrameHeight = "map_frame_height";
static const std::string colMapFloor = "map_floor";
}  // exhibits

namespace reports {
static const std::string tableName = "reports";

static const std::string colId = "id";
static const std::string colExperimentId = "experiment_id";
static const std::string colDocument = "doc";

// json
// field[level]_[name]
static const std::string field0_History = "history";

static const std::string field00_ExhibitId = "exhibitId";
static const std::string field01_DurationInSecs = "durationInSecs";
static const std::string field02_Actions = "actions";

static const std::string field1_SurveyBefore = "surveyBefore";
static const std::string field1_SurveyAfter = "surveyAfter";

static const std::string field10_SimpleQuestions = "simpleQuestions";
static const std::string field100_Answer = "answer";
static const std::string field11_MultipleChoiceQuestions = "multipleChoiceQuestions";
static const std::string field110_Answer = "answer";
static const std::string field12_SortQuestions = "sortQuestions";
static const std::string field120_Answer = "answer";

}  // reports

namespace actions {
static const std::string tableName = "actions";

static const std::string colId = "id";
static const std::string colText = "text";
}  // actions

namespace simple_questions {
static const std::string tableName = "simple_questions";

static const std::string colId = "id";
static const std::string colName = "name";
static const std::string colQuestion = "question";
static const std::string colNumberAnswer = "number_answer";
}  // simple questions

namespace multiple_choice_questions {
static const std::string tableName = "multiple_choice_questions";

static const std::string colId = "id";
static const std::string colName = "name";
static const std::string colQuestion = "question";
static const std::string colSingleAnswer = "single_answer";
}  // multiple choice questions

namespace multiple_choice_question_options {
static const std::string tableName = "multiple_choice_question_options";

static const std::string colId = "id";
static const std::string colQuestionId = "question_id";
static const std::string colText = "text";
}  // multiple choice question options

namespace sort_questions {
static const std::string tableName = "sort_questions";

static const std::string colId = "id";
static const std::string colName = "name";
static const std::string colQuestion = "question";
}  // sort questions

namespace sort_question_options {
static const std::string tableName = "sort_question_options";

static const std::string colId = "id";
static const std::string colQuestionId = "question_id";
static const std::string colText = "text";
}  // sort question options

namespace experiments {
namespace current {
static const std::string tableName = "active_experiment";
static const std::string colId = "id";
}  // current

static const std::string tableName = "experiments";

static const std::string colId = "id";
static const std::string colName = "name";
static const std::string colDocument = "doc";

// json
// field[level]_[name]
static const std::string field0_Actions = "actions";
static const std::string field1_BreakActions = "breakActions";

static const std::string field2_SurveyBefore = "surveyBefore";
static const std::string field2_SurveyAfter = "surveyAfter";
static const std::string field20_TypesOrder = "typesOrder";
static const std::string field21_SimpleQuestions = "simpleQuestions";
static const std::string field22_MultipleChoiceQuestions = "multipleChoiceQuestions";
static const std::string field23_SortQuestions = "sortQuestions";
}  // experiments
}
}

#endif