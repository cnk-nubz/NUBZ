#!/bin/bash

mkdir -p server/src/communication
thrift -r -out server/src/communication --gen cpp communication/server.thrift 
rm server/src/communication/Server_server.skeleton.cpp 

mkdir -p www/cnk_admin_site/thrift_communication/thrift_structures
thrift -r -out www/cnk_admin_site/thrift_communication/thrift_structures --gen py communication/server.thrift
mv www/cnk_admin_site/thrift_communication/thrift_structures/server/Server-remote www/cnk_admin_site/thrift_communication/thrift_structures/

mkdir -p android/prj/app/src/main/java/
thrift -r -out android/prj/app/src/main/java --gen java communication/server.thrift 

