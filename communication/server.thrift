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

	structs.MapImageTilesResponse getMapImageTiles(1: structs.MapImageTilesRequest request)
		throws (1: structs.InternalError err), 

	void setMapImage(1: structs.SetMapImageRequest request)
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


/////////////////////////////////////////////////
// Experiments
/////////////////////////////////////////////////

	structs.CurrentExperimentResponse getCurrentExperiment()
		throws (1: structs.InternalError err),


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
// Simple Question
/////////////////////////////////////////////////

	structs.SimpleQuestion createSimpleQuestion(1: structs.CreateSimpleQuestionRequest request)
		throws (1: structs.InternalError intErr, 2: structs.InvalidData dataErr),

	list<structs.SimpleQuestion> getAllSimpleQuestions()
		throws (1: structs.InternalError err),
}