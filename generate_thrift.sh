#!/bin/bash

thrift -r -o server/src/ --gen cpp communication/server.thrift 
rm server/src/gen-cpp/Server_server.skeleton.cpp 

thrift -r -o www/src/ --gen py communication/server.thrift
mv www/src/gen-py/server/Server-remote www/src/gen-py/

thrift -r -o android/src/ --gen java communication/server.thrift 

