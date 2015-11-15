package com.cnk.communication;

import android.app.IntentService;
import android.util.Log;

import com.cnk.utilities.Util;

import org.apache.thrift.transport.TSocket;
import org.apache.thrift.transport.TTransport;

public abstract class ServerConnector extends IntentService {

    protected static final String LOG_TAG = "ServerConnector";
    protected static final long DELAY = 30000;
    private static final String SEND_ADDRESS = "localhost";
    private static final int SEND_PORT = 6969;

    protected TTransport socket;

    public ServerConnector() {
        super(LOG_TAG);
    }

    protected int openSocket(int tries) {
        socket = new TSocket(SEND_ADDRESS, SEND_PORT);
        Log.i(LOG_TAG, "opening socket");
        boolean opened = false;
        while (!opened && tries > 0) {
            try {
                socket.open();
                opened = true;
                Log.i(LOG_TAG, "opened socket");
                return tries;
            } catch (org.apache.thrift.transport.TTransportException transportException) {
                tries--;
                Log.e(LOG_TAG, "socket open failed, remaining tries: " + Integer.toString(tries));
                Util.waitDelay(DELAY);
            }
        }
        return tries;
    }

}
