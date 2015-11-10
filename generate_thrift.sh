#!/bin/bash

thrift -r -out server/src/communication --gen cpp communication/server.thrift 
rm server/src/communication/Server_server.skeleton.cpp 

thrift -r -out www/cnk/strona/thrift_structures --gen py communication/server.thrift
mv www/cnk/strona/thrift_structures/server/Server-remote www/cnk/strona/thrift_structures/

thrift -r -out android/prj/app/src/main/java --gen java communication/server.thrift 

