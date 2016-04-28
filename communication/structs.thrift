namespace cpp communication
namespace java com.cnk.communication.thrift

typedef string Filename

/////////////////////////////////////////////////
// Exceptions
/////////////////////////////////////////////////

exception InternalError {
}

exception InvalidData {
}

exception DuplicateName {
}


/////////////////////////////////////////////////
// Ping
/////////////////////////////////////////////////

struct HelloMsg {
	1: required i32 num,
	2: required string msg,
}


/////////////////////////////////////////////////
// Map
/////////////////////////////////////////////////

struct Size {
	1: required i32 width,
	2: required i32 height,
}

// (x, y) = left upper corner
struct Frame {
	1: required i32 x,
	2: required i32 y,
	3: required Size size,
}

struct MapFrame {
	1: required Frame frame,
	2: required i32 floor,
}

struct ZoomLevel {
	1: required Size scaledSize,
	2: required Size tileSize,
	3: required list<list<string>> tilesUrls,
}

struct MapImage {
	1: required i32 floor,
	2: required Size resolution,
	3: required list<ZoomLevel> zoomLevels,
}

struct NewMapImagesRequest {
	1: optional i32 acquiredVersion,
}

struct NewMapImagesResponse {
	1: required i32 version,
	2: required map<i32, MapImage> floors,
}

struct SetMapImageRequest {
	1: required i32 floor,
	2: required Filename filename,
}


/////////////////////////////////////////////////
// Exhibits
/////////////////////////////////////////////////

struct Exhibit {
	1: required i32 exhibitId,
	2: required string name,
	3: required i32 rgbHex,
	4: optional MapFrame mapFrame,
}

struct NewExhibitsRequest {
	1: optional i32 acquiredVersion,
}

struct NewExhibitsResponse {
	1: required i32 version,
	2: required map<i32, Exhibit> exhibits,
	3: required bool fullRefresh,
}

struct SetExhibitFrameRequest {
	1: required i32 exhibitId,
	2: required Frame frame,
}

struct CreateExhibitRequest {
	1: required string name,
	2: required i32 rgbHex,
	3: optional i32 floor,
	4: optional MapFrame visibleFrame,
}

struct UpdateExhibitRequest {
	1: required i32 exhibitId,
	2: required i32 rgbHex,
	3: optional i32 floor,
	4: optional MapFrame visibleFrame,
}


/////////////////////////////////////////////////
// Actions
/////////////////////////////////////////////////

struct Action {
	1: required i32 actionId,
	2: required string text,
}

struct CreateActionRequest {
	1: required string text,
}


/////////////////////////////////////////////////
// Simple Question
/////////////////////////////////////////////////

enum SimpleQuestionAnswerType {
	TEXT,
	NUMBER,
}

struct SimpleQuestion {
	1: required i32 questionId,
	2: required string name,
	3: required string question,
	4: required SimpleQuestionAnswerType answerType,
}

struct SimpleQuestionAnswer {
	1: required string answer,
}

// name is optional, null means name == question
struct CreateSimpleQuestionRequest {
	1: required string name,
	2: required string question,
	3: required SimpleQuestionAnswerType answerType,
}


/////////////////////////////////////////////////
// Multiple Choice Question
/////////////////////////////////////////////////

struct MultipleChoiceQuestionOption {
	1: required i32 optionId,
	2: required string text,
}

struct MultipleChoiceQuestion {
	1: required i32 questionId,
	2: required string name,
	3: required string question,
	4: required bool singleAnswer,
	5: required list<MultipleChoiceQuestionOption> options,
}

struct MultipleChoiceQuestionAnswer {
	1: required list<i32> choosenOptions,
}

// name is optional, null means name == question
struct CreateMultipleChoiceQuestionRequest {
	1: required string name,
	2: required string question,
	3: required bool singleAnswer,
	4: required list<string> options,
}


/////////////////////////////////////////////////
// Sort Question
/////////////////////////////////////////////////

struct SortQuestionOption {
	1: required i32 optionId,
	2: required string text,
}

struct SortQuestion {
	1: required i32 questionId,
	2: required string name,
	3: required string question,
	4: required list<SortQuestionOption> options,
}

struct SortQuestionAnswer {
	1: required list<i32> choosenOrder,
}

// name is optional, null means name == question
struct CreateSortQuestionRequest {
	1: required string name,
	2: required string question,
	3: required list<string> options,
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
	1: required list<QuestionType> questionsOrder,
	2: required list<SimpleQuestion> simpleQuestions,
	3: required list<MultipleChoiceQuestion> multipleChoiceQuestions,
	4: required list<SortQuestion> sortQuestions,
}

struct QuestionsIdsList {
	1: required list<QuestionType> questionsOrder,
	2: required list<i32> simpleQuestions,
	3: required list<i32> multipleChoiceQuestions,
	4: required list<i32> sortQuestions,
}


/////////////////////////////////////////////////
// Experiment
/////////////////////////////////////////////////

struct Date {
	1: required i32 day,
	2: required i32 month,
	3: required i32 year,
}

struct Experiment {
	1: required i32 experimentId,
	2: required string name,
	3: required QuestionsList surveyBefore,
	4: required list<Action> exhibitActions,
	5: required list<Action> breakActions,
	6: required QuestionsList surveyAfter,
}

struct CurrentExperimentResponse {
	1: optional Experiment experiment,
}

struct CreateExperimentRequest {
	1: required string name,
	2: required QuestionsIdsList surveyBefore,
	3: required list<i32> exhibitActions,
	4: required list<i32> breakActions,
	5: required QuestionsIdsList surveyAfter,
}

struct ExperimentInfo {
	1: required i32 id,
	2: required string name,
	3: optional Date startDate,
	4: optional Date finishDate,
}

struct SingleExperimentInfo {
	1: optional ExperimentInfo info,
}

struct CloneRequest {
	1: required i32 id,
	2: required string name,
}


/////////////////////////////////////////////////
// Reports
/////////////////////////////////////////////////

// 00:00:00 - 23:59:59
struct Time {
	1: required i32 hour,
	2: required i32 min,
	3: required i32 sec,
}

struct ReportInfo {
	1: required i32 reportId,
	2: required Date receiveDate,
}

// exhibitID is optional, null means break
struct RawReportEvent {
	1: optional i32 exhibitId,
	2: required Time beginTime,
	3: required i32 durationInSecs,
	4: required list<i32> actions,
}

struct SurveyAnswers {
	1: required list<SimpleQuestionAnswer> simpleQuestionsAnswers,
	2: required list<MultipleChoiceQuestionAnswer> multipleChoiceQuestionsAnswers,
	3: required list<SortQuestionAnswer> sortQuestionsAnswers,
}

struct RawReport {
	1: required i32 experimentId,
	2: required i32 reportId,
	3: required SurveyAnswers answersBefore,
	4: required list<RawReportEvent> history,
	5: required SurveyAnswers answersAfter,
	6: required Time beginTime,
	7: required Time finishTime,
}