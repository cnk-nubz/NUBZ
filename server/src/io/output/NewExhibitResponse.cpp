#include "NewExhibitResponse.h"

namespace io {
    namespace output {
        communication::NewExhibitResponse NewExhibitResponse::toThrift() const {
            communication::NewExhibitResponse thrift;
            thrift.exhibitId = exhibitId;
            thrift.exhibit = exhibit.toThrift();
            return thrift;
        }
    }
}