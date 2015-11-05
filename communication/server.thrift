include "structs.thrift"

namespace cpp communication
namespace java communication

service Server {
   i32 ping(1: structs.HelloMsg msg),
}