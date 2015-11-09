#!/bin/bash
if [ -z "$1" ]
	then echo "error: path to sdk not provided"
	exit
fi
echo "sdk.dir="$1 > prj/local.properties
set ANDROID_HOME=$1
source ~/.bash_profile
cd prj
./gradlew assemble
cd ..
