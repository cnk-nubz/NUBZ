#!/bin/bash

set -e

# server - download
wget -O /tmp/nubz.zip https://github.com/easylogging/easyloggingpp/releases/download/v9.80/easyloggingpp_v9.80.zip

# server - unpack
mkdir -p server/external
unzip -o /tmp/nubz.zip -d server/external

