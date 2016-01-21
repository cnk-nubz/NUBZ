include "structs.thrift"

namespace cpp communication
namespace java com.cnk.communication

service Server {
	void shutdown(),
	i32 ping(1: structs.HelloMsg msg),

	structs.MapImagesResponse getMapImages(1: structs.MapImagesRequest request)
		throws (1: structs.InternalError err),

	structs.MapImageTilesResponse getMapImageTiles(1: structs.MapImageTilesRequest request)
		throws (1: structs.InternalError err), 

	void setMapImage(1: structs.SetMapImageRequest request)
		throws (1: structs.InternalError intErr, 2: structs.InvalidData dataErr),

	structs.ExhibitsResponse getExhibits(1: structs.ExhibitsRequest request)
		throws (1: structs.InternalError err),

	i32 getIdForNewReport()
		throws (1: structs.InternalError err),

	void saveReport(1: structs.RawReport report)
		throws (1: structs.InternalError intErr, 2: structs.InvalidData dataErr),

	void setExhibitFrame(1: structs.SetExhibitFrameRequest request)
		throws (1: structs.InternalError intErr, 2: structs.InvalidData dataErr),
		
	structs.CurrentExperimentResponse getCurrentExperiment()
		throws (1: structs.InternalError err),

	structs.NewExhibitResponse createNewExhibit(1: structs.NewExhibitRequest request)
		throws (1: structs.InternalError intErr, 2: structs.InvalidData dataErr),

	structs.NewActionResponse createNewAction(1: structs.NewActionRequest request)
		throws (1: structs.InternalError intErr, 2: structs.InvalidData dataErr),

	structs.ActionsResponse getActions()
		throws (1: structs.InternalError err),
}