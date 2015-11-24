#ifndef IO__EXHIBIT__H
#define IO__EXHIBIT__H

#include <string>

#include <boost/optional.hpp>

#include "communication/structs_types.h"
#include "io/MapElementFrame.h"
#include "db/struct/Exhibit.h"

namespace io {
    struct Exhibit {
        Exhibit() = default;
        ~Exhibit() = default;

        Exhibit(const Exhibit &) = default;
        Exhibit(Exhibit &&) = default;
        Exhibit &operator=(const Exhibit &) = default;
        Exhibit &operator=(Exhibit &&) = default;

        Exhibit(const communication::Exhibit &thrift);
        communication::Exhibit toThrift() const;

        Exhibit(const db::Exhibit &dbExhibit);
        db::Exhibit toDb() const;

        std::string name;
        boost::optional<MapElementFrame> frame;
    };
}

#endif