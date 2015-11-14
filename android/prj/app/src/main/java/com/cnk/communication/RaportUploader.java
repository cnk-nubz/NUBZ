package com.cnk.communication;

import android.app.IntentService;
import android.content.Intent;
import android.util.Log;

import com.cnk.utilities.Util;

import org.apache.thrift.TException;
import org.apache.thrift.protocol.TBinaryProtocol;
import org.apache.thrift.protocol.TProtocol;
import org.apache.thrift.transport.TSocket;
import org.apache.thrift.transport.TTransport;

public class RaportUploader extends IntentService {
    private static final String LOG_TAG = "ReportUploader";
    private static final long DELAY = 30000;
    private static String SEND_ADDRESS = "";
    private static int SEND_PORT = 0;
    private TTransport socket;

    public RaportUploader() {
        super("RaportUploader");
    }

    public static void setEndpoint(String address, int port) {
        SEND_ADDRESS = address;
        SEND_PORT = port;
    }

    @Override
    protected void onHandleIntent(Intent intent) {
        if (intent != null) {
            //TODO extract data from intent
            sendData(/* data */);
        }
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

    private void sendData(/* data */) {
        Log.i(LOG_TAG, "sending");

        openSocket();
        boolean succeeded = false;
        TProtocol protocol = new TBinaryProtocol(socket);
        Server.Client client = new Server.Client(protocol);

        HelloMsg msg = new HelloMsg();
        msg.msg = "test";
        msg.num = 123;

        while (!succeeded) {
            try {
                client.ping(msg);
                succeeded = true;
                Log.i(LOG_TAG, "sent");
            } catch (TException e1) {
                Log.e(LOG_TAG, "sending failed");
                Util.waitDelay(DELAY);
            }
        }
        socket.close();
        socket = null;
    }

}
