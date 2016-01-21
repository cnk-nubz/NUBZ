namespace cpp communication
namespace java com.cnk.communication

exception InternalError {
}

exception InvalidData {
}

struct HelloMsg {
	1: i32 num,
	2: string msg,
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

struct Size {
	1: i32 width,
	2: i32 height,
}

struct ImageTiles {
	1: Size scaledSize,
	2: Size tileSize,
	3: list<list<string>> tilesUrls,
}

struct MapImageTilesResponse {
	1: Size originalSize,
	2: list<ImageTiles> zoomLevels,
}

struct SetMapImageRequest {
	1: i32 level
	2: string filename
}

struct ExhibitsRequest {
	1: optional i32 acquiredVersion,
}

// (x, y) = left upper corner
struct MapElementFrame {
	1: i32 x,
	2: i32 y,
	3: i32 width,
	4: i32 height,
	5: i32 mapLevel,
}

struct Exhibit {
	1: string name,
	2: optional MapElementFrame frame,
}

struct ExhibitsResponse {
	1: i32 version,
	2: map<i32, Exhibit> exhibits,
}

// exhibitID is optional, null means break
struct RawReportEvent {
	1: optional i32 exhibitId,
	2: i32 durationInSecs,
	3: list<i32> actions,
}

struct RawReport {
	1: i32 reportId,
	2: list<RawReportEvent> history,
}

struct SetExhibitFrameRequest {
	1: i32 exhibitId,
	2: i32 x,
	3: i32 y,
	4: i32 width,
	5: i32 height,
}

struct Action {
	1: i32 actionId,
	2: string text,
}

struct Experiment {
	1: i32 experimentId,
	2: string name,
	3: list<Action> exhibitActions,
	4: list<Action> breakActions,
}

struct CurrentExperimentResponse {
	1: optional Experiment experiment,
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

struct NewActionRequest {
	1: string text,
}

struct NewActionResponse {
	1: Action action,
}

struct ActionsResponse {
	1: list<Action> actions,
}
