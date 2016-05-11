#!/bin/bash

set -e

cp images/map0.jpg www/download_tmp/;
cp images/map1.jpg www/download_tmp/;

cd www/cnk_admin_site/thrift_structures/;
./Server-remote -h localhost:9090 -f setMapImage 'structs.ttypes.SetMapImageRequest(0, "map0.jpg")' > /dev/null;
./Server-remote -h localhost:9090 -f setMapImage 'structs.ttypes.SetMapImageRequest(1, "map1.jpg")' > /dev/null;
