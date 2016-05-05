#!/bin/bash

set -e

./utils/create_directories.sh
./utils/generate_thrift.sh
./utils/download_external_files.sh
