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
}