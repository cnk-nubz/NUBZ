namespace cpp communication
namespace java com.cnk.communication.thrift

/////////////////////////////////////////////////
// Exceptions
/////////////////////////////////////////////////

exception InternalError {
}

exception InvalidData {
}


/////////////////////////////////////////////////
// Ping
/////////////////////////////////////////////////

struct HelloMsg {
	1: i32 num,
	2: string msg,
}


/////////////////////////////////////////////////
// Map
/////////////////////////////////////////////////

struct Size {
	1: i32 width,
	2: i32 height,
}

// (x, y) = left upper corner
struct Frame {
	1: i32 x,
	2: i32 y,
	3: Size size,
}

struct MapFrame {
	1: Frame frame,
	2: i32 floor,
}

struct ImageTiles {
	1: Size scaledSize,
	2: Size tileSize,
	3: list<list<string>> tilesUrls,
}

struct NewMapImagesRequest {
	1: optional i32 acquiredVersion,
}

struct NewMapImagesResponse {
	1: i32 version,
	2: map<i32, string> floorImageUrls,
}

struct MapImageTilesRequest {
	1: i32 floor,
}

struct MapImageTilesResponse {
	1: Size originalSize,
	2: list<ImageTiles> zoomLevels,
}

struct SetMapImageRequest {
	1: i32 floor,
	2: string filename,
}


/////////////////////////////////////////////////
// Exhibits
/////////////////////////////////////////////////

struct Exhibit {
	1: i32 exhibitId,
	2: string name,
	3: optional MapFrame mapFrame,
}

struct NewExhibitsRequest {
	1: optional i32 acquiredVersion,
}

struct NewExhibitsResponse {
	1: i32 version,
	2: map<i32, Exhibit> exhibits,
}

struct SetExhibitFrameRequest {
	1: i32 exhibitId,
	2: Frame frame,
}

struct CreateExhibitRequest {
	1: string name,
	2: optional i32 floor,
	3: optional MapFrame visibleFrame,
}


/////////////////////////////////////////////////
// Actions
/////////////////////////////////////////////////

struct Action {
	1: i32 actionId,
	2: string text,
}

struct CreateActionRequest {
	1: string text,
}


/////////////////////////////////////////////////
// Simple Question
/////////////////////////////////////////////////

enum SimpleQuestionAnswerType {
	TEXT,
	NUMBER,
}

struct SimpleQuestion {
	1: i32 questionId,
	2: string name,
	3: string question,
	4: SimpleQuestionAnswerType answerType,
}

struct SimpleQuestionAnswer {
	1: optional string answer,
}

// name is optional, null means name == question
struct CreateSimpleQuestionRequest {
	1: optional string name,
	2: string question,
	3: SimpleQuestionAnswerType answerType,
}


/////////////////////////////////////////////////
// Multiple Choice Question
/////////////////////////////////////////////////

struct MultipleChoiceQuestionOption {
	1: i32 optionId,
	2: string text,
}

struct MultipleChoiceQuestion {
	1: i32 questionId,
	2: string name,
	3: string question,
	4: bool singleAnswer,
	5: list<MultipleChoiceQuestionOption> options,
}

struct MultipleChoiceQuestionAnswer {
	1: optional list<i32> choosenOptions,
}

// name is optional, null means name == question
struct CreateMultipleChoiceQuestionRequest {
	1: optional string name,
	2: string question,
	3: bool singleAnswer,
	4: list<string> options,
}


/////////////////////////////////////////////////
// Sort Question
/////////////////////////////////////////////////

struct SortQuestionOption {
	1: i32 optionId,
	2: string text,
}

struct SortQuestion {
	1: i32 questionId,
	2: string name,
	3: string question,
	4: list<SortQuestionOption> options,
}

struct SortQuestionAnswer {
	1: optional list<i32> choosenOrder,
}

// name is optional, null means name == question
struct CreateSortQuestionRequest {
	1: optional string name,
	2: string question,
	3: list<string> options,
}


/////////////////////////////////////////////////
// Questions List
/////////////////////////////////////////////////

enum QuestionType {
	SIMPLE,
	MULTIPLE_CHOICE,
	SORT,
}

struct QuestionsList {
	1: list<QuestionType> questionsOrder,
	2: list<SimpleQuestion> simpleQuestions,
	3: list<MultipleChoiceQuestion> multipleChoiceQuestions,
	4: list<SortQuestion> sortQuestions,
}


/////////////////////////////////////////////////
// Experiment
/////////////////////////////////////////////////

struct Experiment {
	1: i32 experimentId,
	2: string name,
	3: QuestionsList surveyBefore,
	4: list<Action> exhibitActions,
	5: list<Action> breakActions,
	6: QuestionsList surveyAfter,
}

struct CurrentExperimentResponse {
	1: optional Experiment experiment,
}


/////////////////////////////////////////////////
// Reports
/////////////////////////////////////////////////

// exhibitID is optional, null means break
struct RawReportEvent {
	1: optional i32 exhibitId,
	2: i32 durationInSecs,
	3: list<i32> actions,
}

struct SurveyAnswers {
	1: list<SimpleQuestionAnswer> simpleQuestionsAnswers,
	2: list<MultipleChoiceQuestionAnswer> multipleChoiceQuestionsAnswers,
	3: list<SortQuestionAnswer> sortQuestionsAnswers,
}

struct RawReport {
	1: i32 experimentId,
	2: i32 reportId,
	3: SurveyAnswers answersBefore,
	4: list<RawReportEvent> history,
	5: SurveyAnswers answersAfter,
}