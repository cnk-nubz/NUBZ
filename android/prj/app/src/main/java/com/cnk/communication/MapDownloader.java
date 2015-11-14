package com.cnk.communication;

import android.app.IntentService;
import android.content.Intent;
import android.util.Log;

import com.cnk.data.DataHandler;
import com.cnk.utilities.Util;

import org.apache.thrift.TException;
import org.apache.thrift.protocol.TBinaryProtocol;
import org.apache.thrift.protocol.TProtocol;
import org.apache.thrift.transport.TSocket;
import org.apache.thrift.transport.TTransport;

import java.io.IOException;
import java.util.Map;

public class MapDownloader extends IntentService {

    private static final String LOG_TAG = "MapDownlaoder";
    private static DataHandler dataHandler;
    private static String SEND_ADDRESS = "";
    private static int SEND_PORT = 0;
    private static long DELAY = 30000;
    private TTransport socket;

    public MapDownloader() {
        super("MapDownloader");
    }

    public static void setEndpoint(String address, int port) {
        SEND_ADDRESS = address;
        SEND_PORT = port;
    }

    public static void setDataHandler(DataHandler dataHandler) {
        MapDownloader.dataHandler = dataHandler;
    }

    @Override
    protected void onHandleIntent(Intent intent) {
        checkMapsUpdate();
    }

    private void openSocket() {
        socket = new TSocket(SEND_ADDRESS, SEND_PORT);
        Log.i(LOG_TAG, "opening socket");
        boolean opened = false;
        while (!opened) {
            try {
                socket.open();
                opened = true;
                Log.i(LOG_TAG, "opened socket");
            } catch (org.apache.thrift.transport.TTransportException transportException) {
                Log.e(LOG_TAG, "socket open failed");
                Util.waitDelay(DELAY);
            }
        }
    }

    private void checkMapsUpdate() {
        Log.i(LOG_TAG, "sending");
        openSocket();
        TProtocol protocol = new TBinaryProtocol(socket);
        Server.Client client = new Server.Client(protocol);
        MapImagesRequest request = new MapImagesRequest();
        request.acquiredLevel = dataHandler.getMapVersion();
        MapImagesResponse response = null;
        boolean succeeded = false;
        while(!succeeded) {
            try {
                response = client.getMapImages(request);
                succeeded = true;
                Log.i(LOG_TAG, "sent");
            } catch (TException e) {
                Log.e(LOG_TAG, "sending failed");
                e.printStackTrace();
                Util.waitDelay(DELAY);
            }
        }
        dataHandler.setMapVersion(response.version);
        for (Map.Entry<Integer, String> entry : response.levelImageUrls.entrySet()) {
            try {
                dataHandler.setMapFloor(entry.getKey(), entry.getValue());
            } catch (IOException e) {
                Log.e(LOG_TAG, "cant set floor");
                e.printStackTrace();
            }
        }
    }

}
