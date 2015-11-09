package com.cnk.communication;

import android.app.IntentService;
import android.content.Intent;
import android.util.Log;

import org.apache.thrift.TDeserializer;
import org.apache.thrift.TException;
import org.apache.thrift.protocol.TBinaryProtocol;
import org.apache.thrift.protocol.TProtocol;
import org.apache.thrift.transport.TSocket;
import org.apache.thrift.transport.TTransport;

public class RaportUploader extends IntentService {
    private static final String LOG_TAG = "ReportUploader";
    private static final String SEND_ADDRESS = "192.168.1.111";
    private static final int SEND_PORT = 9090;
    private static final long DELAY = 30000;
    private static TTransport socket;
    private static TDeserializer deserializer;

    public RaportUploader() {
        super("RaportUploader");
        socket = new TSocket(SEND_ADDRESS, SEND_PORT);
    }

    @Override
    protected void onHandleIntent(Intent intent) {
        if (intent != null) {
            //TODO extract data from intent
            sendData(/* data */);
        }
    }

    private void openSocket() {
        Log.i(LOG_TAG, "opening socket");
        boolean opened = false;
        while (!opened) {
            try {
                socket.open();
                opened = true;
                Log.i(LOG_TAG, "opened socket");
            } catch (org.apache.thrift.transport.TTransportException transportException) {
                Log.i(LOG_TAG, "socket open failed");
                waitDelay();
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
                Log.i(LOG_TAG, "sending failed");
                waitDelay();
            }
        }
        socket.close();
        socket = null;
    }

    private void waitDelay() {
        try {
            Thread.sleep(DELAY);
        } catch (java.lang.InterruptedException e2) {
            e2.printStackTrace();
        }
    }
}
