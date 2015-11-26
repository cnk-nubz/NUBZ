package com.cnk.communication.task;

import android.util.Log;

import com.cnk.communication.MapImagesRequest;
import com.cnk.communication.MapImagesResponse;
import com.cnk.communication.Server;
import com.cnk.data.DataHandler;
import com.cnk.notificators.Notificator;

import org.apache.thrift.TException;

import java.io.IOException;

public class MapDownloadTask extends ServerTask {

    private static final String LOG_TAG = "MapDownloadTask";
    private static final Integer FLOOR1 = 0;
    private static final Integer FLOOR2 = 1;

    public MapDownloadTask(Notificator notificator) {
        super(notificator);
    }

    public void performInSession(Server.Client client) throws TException, IOException {
        Log.i(LOG_TAG, "Downloading map");
        MapImagesRequest request = new MapImagesRequest();
        if (DataHandler.getInstance().getMapVersion() != null) {
            request.setAcquiredVersion(DataHandler.getInstance().getMapVersion());
        }
        MapImagesResponse response = client.getMapImages(request);
        updateDataHandler(response);
        Log.i(LOG_TAG, "Map downloaded");
    }

    private void updateDataHandler(MapImagesResponse response) throws IOException {
        Integer version = response.getVersion();
        String floor1Url = response.getLevelImageUrls().get(FLOOR1);
        String floor2Url = response.getLevelImageUrls().get(FLOOR2);
        try {
            DataHandler.getInstance().setMaps(version, floor1Url, floor2Url);
        } catch (IOException e) {
            e.printStackTrace();
            throw e;
        }

    }
}
