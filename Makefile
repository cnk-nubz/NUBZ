THRIFT_SRCS = communication/*.thrift
THRIFT_ANDROID = android/prj/app/src/main/java/com/cnk/communication/thrift/Server.java
THRIFT_SERVER = server/src/communication/Server.cpp
THRIFT_WWW = www/cnk_admin_site/thrift_communication/thrift_structures/server/Server.py

EXTERNAL_TMP = /tmp/nubz.zip
EXTERNAL_ANDROID = android/prj/app/src/main/java/com/cnk/external
EXTERNAL_SERVER = server/external
EXTERNAL_WWW = www/cnk_admin_site/static/include

DOWNLOAD_DIR = www/download

setup: dirs thrift external

db: 
	python utils/create_database.py

reset_db:
	python utils/delete_database.py
	python utils/create_database.py

clean_db:
	python utils/delete_database.py

dirs: $(DOWNLOAD_DIR)

$(DOWNLOAD_DIR):
	mkdir -p $(DOWNLOAD_DIR)/{tmp,map_tiles,excel_files,android_app}

thrift: $(THRIFT_ANDROID) $(THRIFT_SERVER) $(THRIFT_WWW)

$(THRIFT_ANDROID): $(THRIFT_SRCS)
	mkdir -p android/prj/app/src/main/java/
	thrift -r -out android/prj/app/src/main/java --gen java communication/server.thrift

$(THRIFT_SERVER): $(THRIFT_SRCS)
	mkdir -p server/src/communication
	thrift -r -out server/src/communication --gen cpp communication/server.thrift 
	rm server/src/communication/Server_server.skeleton.cpp 

$(THRIFT_WWW): $(THRIFT_SRCS)
	mkdir -p www/cnk_admin_site/thrift_communication/thrift_structures
	thrift -r -out www/cnk_admin_site/thrift_communication/thrift_structures --gen py communication/server.thrift
	mv www/cnk_admin_site/thrift_communication/thrift_structures/server/Server-remote www/cnk_admin_site/thrift_communication/thrift_structures/

external: $(EXTERNAL_ANDROID) $(EXTERNAL_SERVER) $(EXTERNAL_WWW)

$(EXTERNAL_ANDROID):
	wget -O $(EXTERNAL_TMP) https://github.com/gawronp/AutoResizeTextView/archive/2.1.zip
	mkdir -p $(EXTERNAL_ANDROID)
	unzip -j -o $(EXTERNAL_TMP) -d $(EXTERNAL_ANDROID)
	rm -rf $(EXTERNAL_TMP)

$(EXTERNAL_SERVER):
	wget -O $(EXTERNAL_TMP) https://github.com/easylogging/easyloggingpp/releases/download/v9.80/easyloggingpp_v9.80.zip
	mkdir -p $(EXTERNAL_SERVER)
	unzip -o $(EXTERNAL_TMP) -d $(EXTERNAL_SERVER)
	rm -rf $(EXTERNAL_TMP)

$(EXTERNAL_WWW):
	mkdir -p $(EXTERNAL_WWW)/{js,css,tmp/js}
	wget -O $(EXTERNAL_TMP) https://github.com/ThatKorean/Leaflet/releases/download/v1.0.0-rc.1-no-anim/leaflet-min.zip
	unzip -o $(EXTERNAL_TMP) -d $(EXTERNAL_WWW)/tmp
	wget -O $(EXTERNAL_TMP) https://github.com/ThatKorean/Leaflet.label/releases/download/0.2.1-min-all/Leaflet.Label.min.zip
	unzip -o $(EXTERNAL_TMP) -d $(EXTERNAL_WWW)/tmp
	wget -O $(EXTERNAL_TMP) https://github.com/ThatKorean/Leaflet.Path.Drag/releases/download/0.1.3-all-in-one/Leaflet.Path.Drag.min.js.zip
	unzip -o $(EXTERNAL_TMP) -d $(EXTERNAL_WWW)/tmp/js
	wget -O $(EXTERNAL_TMP) https://github.com/ThatKorean/Leaflet.draw/releases/download/v0.2.4-in-one/Leaflet.Draw.min.js.zip
	unzip -o $(EXTERNAL_TMP) -d $(EXTERNAL_WWW)/tmp/js
	cat $(EXTERNAL_WWW)/tmp/js/leaflet.min.js $(EXTERNAL_WWW)/tmp/js/Leaflet.Label.min.js $(EXTERNAL_WWW)/tmp/js/Leaflet.Draw.min.js $(EXTERNAL_WWW)/tmp/js/Leaflet.Path.Drag.min.js >> $(EXTERNAL_WWW)/js/leaflet-all.min.js
	cat $(EXTERNAL_WWW)/tmp/css/leaflet.css $(EXTERNAL_WWW)/tmp/css/leaflet.label.css >> $(EXTERNAL_WWW)/css/leaflet-all.min.css
	rm -Rf $(EXTERNAL_WWW)/tmp
	rm -rf $(EXTERNAL_TMP)

clean:
	git clean -d -f .
