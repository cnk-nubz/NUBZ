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

    public void run(int tries) {
        Log.i(LOG_TAG, "sending");
        tries = openSocket(tries);
        TProtocol protocol = new TBinaryProtocol(socket);
        Server.Client client = new Server.Client(protocol);

        MapImagesRequest request = new MapImagesRequest();
        if (DataHandler.getInstance().getMapVersion() != null) {
            request.acquiredLevel = DataHandler.getInstance().getMapVersion();
        }
        MapImagesResponse response = null;

        boolean succeeded = false;
        while(!succeeded && tries > 0) {
            try {
                response = client.getMapImages(request);
                succeeded = true;
                updateDataHandler(response);
                Log.i(LOG_TAG, "sent");
            } catch (TException e) {
                Log.e(LOG_TAG, "sending failed, remaining tries: " + Integer.toString(tries));
                e.printStackTrace();
                Util.waitDelay(DELAY);
            }
        }

        if (tries <= 0) {
            notificator.failure();
        } else {
            notificator.success();
        }
        socket.close();
        Log.i(LOG_TAG, "exiting");
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
