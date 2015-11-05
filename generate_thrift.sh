#!/bin/bash

thrift -r -out server/src/communication --gen cpp communication/server.thrift 
rm server/src/communication/Server_server.skeleton.cpp 

thrift -r -o www/src/ --gen py communication/server.thrift
mv www/src/gen-py/server/Server-remote www/src/gen-py/

thrift -r -out android/prj/app/src/main/java --gen java communication/server.thrift 

