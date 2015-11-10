include "structs.thrift"

namespace cpp communication
namespace java com.cnk.communication

service Server {
	void shutdown(),
	i32 ping(1: structs.HelloMsg msg),
}