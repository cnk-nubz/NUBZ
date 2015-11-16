package com.cnk.communication.task;

import android.content.Context;
import android.util.Log;

import com.cnk.notificators.Notificator;
import com.cnk.utilities.Util;

import org.apache.thrift.transport.TSocket;
import org.apache.thrift.transport.TTransport;

public abstract class ServerTask extends Task {

    protected static final String LOG_TAG = "ServerTask";
    protected static final long DELAY = 30000;
    private static final String SEND_ADDRESS = "192.168.0.5";
    private static final int SEND_PORT = 9090;

    protected Notificator notificator;
    protected TTransport socket;

    public ServerTask(Notificator notificator) {
        this.notificator = notificator;
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
