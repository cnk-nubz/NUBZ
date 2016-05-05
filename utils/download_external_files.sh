#!/bin/bash

set -e

# server - download
wget -O /tmp/nubz.zip https://github.com/easylogging/easyloggingpp/releases/download/v9.80/easyloggingpp_v9.80.zip

# server - unpack
mkdir -p server/external
unzip -o /tmp/nubz.zip -d server/external

# android - download
wget -O /tmp/AutoResizeTextView.zip https://github.com/gawronp/AutoResizeTextView/archive/2.1.zip

# android - unpack
mkdir -p android/prj/app/src/main/java/com/cnk/external
unzip -j -o /tmp/AutoResizeTextView.zip -d android/prj/app/src/main/java/com/cnk/external

