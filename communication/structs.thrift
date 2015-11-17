namespace cpp communication
namespace java com.cnk.communication

exception InternalError {
}

struct HelloMsg {
	1: i32 num,
	2: string msg,
}

struct MapImagesRequest {
	1: optional i32 acquiredLevel,
}

struct MapImagesResponse {
	1: i32 version,
	2: map<i32, string> levelImageUrls,
}

struct SetMapImageRequest {
	1: i32 level
	2: string filename
}