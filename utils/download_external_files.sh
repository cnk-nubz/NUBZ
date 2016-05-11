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

# www - download
wget -O /tmp/leaflet.zip https://github.com/ThatKorean/Leaflet/releases/download/v1.0.0-rc.1-no-anim/leaflet-min.zip

wget -O /tmp/leaflet-draw.zip https://github.com/ThatKorean/Leaflet.draw/releases/download/v0.2.4-in-one/Leaflet.Draw.min.js.zip

wget -O /tmp/leaflet-labels.zip https://github.com/ThatKorean/Leaflet.label/releases/download/0.2.1-min-all/Leaflet.Label.min.zip

wget -O /tmp/leaflet-pathdrag.zip https://github.com/ThatKorean/Leaflet.Path.Drag/releases/download/0.1.3-all-in-one/Leaflet.Path.Drag.min.js.zip

# www - unpack
lpath='www/cnk_admin_site/static/include'
mkdir -p $lpath/js
unzip -o /tmp/leaflet.zip -d $lpath
unzip -o /tmp/leaflet-labels.zip -d $lpath
unzip -o /tmp/leaflet-draw.zip -d $lpath/js
unzip -o /tmp/leaflet-pathdrag.zip -d $lpath/js 
# merge leaflet js files
cat $lpath/js/leaflet.min.js $lpath/js/Leaflet.Label.min.js $lpath/js/Leaflet.Draw.min.js $lpath/js/Leaflet.Path.Drag.min.js >> $lpath/js/leaflet-all.min.js
# merge leaflet css files
cat $lpath/css/leaflet.css $lpath/css/leaflet.label.css >> $lpath/css/leaflet-all.min.css
# delete files
rm $lpath/js/leaflet.min.js $lpath/js/Leaflet.Label.min.js $lpath/js/Leaflet.Draw.min.js $lpath/js/Leaflet.Path.Drag.min.js
rm $lpath/css/leaflet.css $lpath/css/leaflet.label.css
