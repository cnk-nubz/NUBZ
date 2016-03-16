include "structs.thrift"

namespace cpp communication
namespace java com.cnk.communication.thrift

service Server {
	void shutdown(),


/////////////////////////////////////////////////
// Ping
/////////////////////////////////////////////////

	i32 ping(1: structs.HelloMsg msg),


/////////////////////////////////////////////////
// Map
/////////////////////////////////////////////////

	structs.NewMapImagesResponse getNewMapImages(1: structs.NewMapImagesRequest request)
		throws (1: structs.InternalError err),

	structs.MapImage setMapImage(1: structs.SetMapImageRequest request)
		throws (1: structs.InternalError intErr, 2: structs.InvalidData dataErr),


/////////////////////////////////////////////////
// Exhibits
/////////////////////////////////////////////////

	structs.NewExhibitsResponse getNewExhibits(1: structs.NewExhibitsRequest request)
		throws (1: structs.InternalError err),

	structs.Exhibit createExhibit(1: structs.CreateExhibitRequest request)
		throws (1: structs.InternalError intErr, 2: structs.InvalidData dataErr),

	list<structs.Exhibit> getAllExhibits()
		throws (1: structs.InternalError err),

	void setExhibitFrame(1: structs.SetExhibitFrameRequest request)
		throws (1: structs.InternalError intErr, 2: structs.InvalidData dataErr),

	structs.Exhibit updateExhibit(1: structs.UpdateExhibitRequest request)
		throws (1: structs.InternalError intErr, 2: structs.InvalidData dataErr),


/////////////////////////////////////////////////
// Experiments
/////////////////////////////////////////////////

	structs.CurrentExperimentResponse getCurrentExperiment()
		throws (1: structs.InternalError err),

	void createExperiment(1: structs.CreateExperimentRequest request)
		throws (1: structs.InternalError intErr, 2: structs.InvalidData dataErr),

	list<structs.ExperimentInfo> getReadyExperiments()
		throws (1: structs.InternalError err),

	list<structs.ExperimentInfo> getFinishedExperiments()
		throws (1: structs.InternalError err),

	structs.SingleExperimentInfo getActiveExperiment()
		throws (1: structs.InternalError err),

	structs.Experiment getExperiment(1: i32 experimentId)
		throws (1: structs.InternalError intErr, 2: structs.InvalidData dataErr),

	void startExperiment(1: i32 experimentId)
		throws (1: structs.InternalError intErr, 2: structs.InvalidData dataErr),		

	void finishExperiment()
		throws (1: structs.InternalError intErr, 2: structs.InvalidData dataErr),

/////////////////////////////////////////////////
// Reports
/////////////////////////////////////////////////

	i32 getIdForNewReport()
		throws (1: structs.InternalError err),

	void saveReport(1: structs.RawReport report)
		throws (1: structs.InternalError intErr, 2: structs.InvalidData dataErr),
	

/////////////////////////////////////////////////
// Actions
/////////////////////////////////////////////////

	structs.Action createAction(1: structs.CreateActionRequest request)
		throws (1: structs.InternalError intErr, 2: structs.InvalidData dataErr),

	list<structs.Action> getAllActions()
		throws (1: structs.InternalError err),


/////////////////////////////////////////////////
// Questions
/////////////////////////////////////////////////

	structs.QuestionsList getAllQuestions()
		throws (1: structs.InternalError err),


/////////////////////////////////////////////////
// Simple Question
/////////////////////////////////////////////////

	structs.SimpleQuestion createSimpleQuestion(1: structs.CreateSimpleQuestionRequest request)
		throws (1: structs.InternalError intErr, 2: structs.InvalidData dataErr),

	list<structs.SimpleQuestion> getAllSimpleQuestions()
		throws (1: structs.InternalError err),


/////////////////////////////////////////////////
// Multiple Choice Question
/////////////////////////////////////////////////

	structs.MultipleChoiceQuestion createMultipleChoiceQuestion(1: structs.CreateMultipleChoiceQuestionRequest request)
		throws (1: structs.InternalError intErr, 2: structs.InvalidData dataErr),

	list<structs.MultipleChoiceQuestion> getAllMultipleChoiceQuestions()
		throws (1: structs.InternalError err),


/////////////////////////////////////////////////
// Sort Question
/////////////////////////////////////////////////

	structs.SortQuestion createSortQuestion(1: structs.CreateSortQuestionRequest request)
		throws (1: structs.InternalError intErr, 2: structs.InvalidData dataErr),

	list<structs.SortQuestion> getAllSortQuestions()
		throws (1: structs.InternalError err),
}