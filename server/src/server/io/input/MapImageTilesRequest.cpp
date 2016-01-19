#include "MapImageTilesRequest.h"

namespace server {
namespace io {
namespace input {

MapImageTilesRequest::MapImageTilesRequest(const communication::MapImageTilesRequest &thrift)
    : floor(thrift.floor) {
}
}
}
}