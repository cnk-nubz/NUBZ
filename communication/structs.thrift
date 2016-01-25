namespace cpp communication
namespace java com.cnk.communication

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

// (x, y) = left upper corner
struct MapElementFrame {
	1: i32 x,
	2: i32 y,
	3: i32 width,
	4: i32 height,
	5: i32 mapLevel,
}

struct Size {
	1: i32 width,
	2: i32 height,
}

struct ImageTiles {
	1: Size scaledSize,
	2: Size tileSize,
	3: list<list<string>> tilesUrls,
}

struct MapImagesRequest {
	1: optional i32 acquiredVersion,
}

struct MapImagesResponse {
	1: i32 version,
	2: map<i32, string> levelImageUrls,
}

struct MapImageTilesRequest {
	1: i32 floor,
}

struct MapImageTilesResponse {
	1: Size originalSize,
	2: list<ImageTiles> zoomLevels,
}

struct SetMapImageRequest {
	1: i32 level
	2: string filename
}

/////////////////////////////////////////////////
// Exhibits
/////////////////////////////////////////////////

struct Exhibit {
	1: string name,
	2: optional MapElementFrame frame,
}

struct ExhibitsRequest {
	1: optional i32 acquiredVersion,
}

struct ExhibitsResponse {
	1: i32 version,
	2: map<i32, Exhibit> exhibits,
}

struct SetExhibitFrameRequest {
	1: i32 exhibitId,
	2: i32 x,
	3: i32 y,
	4: i32 width,
	5: i32 height,
}

struct NewExhibitRequest {
	1: string name,
	2: optional i32 floor,
	3: optional MapElementFrame visibleMapFrame,
}

struct NewExhibitResponse {
	1: i32 exhibitId,
	2: Exhibit exhibit,
}

/////////////////////////////////////////////////
// Actions
/////////////////////////////////////////////////

struct Action {
	1: i32 actionId,
	2: string text,
}

struct NewActionRequest {
	1: string text,
}

struct NewActionResponse {
	1: Action action,
}

struct ActionsResponse {
	1: list<Action> actions,
}

/////////////////////////////////////////////////
// Simple Question
/////////////////////////////////////////////////

enum SimpleQuestionAnswerType {
	TEXT,
	NUMBER,
}

// name is optional, null means name == question
struct SimpleQuestion {
	1: i32 id,
	2: optional string name,
	3: string question,
	4: SimpleQuestionAnswerType answerType,
}

struct SimpleQuestionAnswer {
	1: optional string answer,
}

struct NewSimpleQuestionRequest {
	1: string name,
	2: string question,
	3: SimpleQuestionAnswerType answerType,
}

struct NewSimpleQuestionResponse {
	1: string name,
	2: string question,
	3: SimpleQuestionAnswerType answerType,
}

struct SimpleQuestionsResponse {
	1: list<SimpleQuestion> questions,
}

/////////////////////////////////////////////////
// Experiment
/////////////////////////////////////////////////

enum QuestionType {
	SIMPLE,
}

struct Experiment {
	1: i32 experimentId,
	2: string name,
	3: list<QuestionType> beforeQuestionsOrder,
	4: list<SimpleQuestion> beforeSimpleQuestions,
	5: list<Action> exhibitActions,
	6: list<Action> breakActions,
	7: list<QuestionType> afterQuestionOrder,
	8: list<SimpleQuestion> afterSimpleQuestions,
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

struct RawReport {
	1: i32 experimentId,
	2: i32 reportId,
	3: list<SimpleQuestionAnswer> beforeSimpleQuestionsAnswers,
	4: list<RawReportEvent> history,
	5: list<SimpleQuestionAnswer> afterSimpleQuestionsAnswers,
}