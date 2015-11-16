package com.cnk.communication.task;

import android.util.Log;

import com.cnk.communication.MapImagesRequest;
import com.cnk.communication.MapImagesResponse;
import com.cnk.communication.Server;
import com.cnk.data.DataHandler;
import com.cnk.notificators.Notificator;
import com.cnk.utilities.Util;

import org.apache.thrift.TException;
import org.apache.thrift.protocol.TBinaryProtocol;
import org.apache.thrift.protocol.TProtocol;

import java.io.IOException;
import java.util.Map;

public class MapDownloadTask extends ServerTask {

    private static final String LOG_TAG = "MapDownloadTask";

    public MapDownloadTask(Notificator notificator) {
        super(notificator);
    }

    public void performInSession(Server.Client client) throws TException {
        Log.i(LOG_TAG, "Downloading map");
        MapImagesRequest request = new MapImagesRequest();
        if (DataHandler.getInstance().getMapVersion() != null) {
            request.acquiredLevel = DataHandler.getInstance().getMapVersion();
        }
        MapImagesResponse response = null;
        response = client.getMapImages(request);
        updateDataHandler(response);
        Log.i(LOG_TAG, "Map downlaoded");
    }

    private void updateDataHandler(MapImagesResponse response) {
        DataHandler.getInstance().setMapVersion(response.version);
        for (Map.Entry<Integer, String> entry : response.levelImageUrls.entrySet()) {
            try {
                DataHandler.getInstance().setMapFloor(entry.getKey(), entry.getValue());
            } catch (IOException e) {
                Log.e(LOG_TAG, "cant set floor");
                e.printStackTrace();
            }
        }
    }
}
