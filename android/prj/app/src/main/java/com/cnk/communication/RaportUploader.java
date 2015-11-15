package com.cnk.communication;

import android.app.IntentService;
import android.content.Intent;
import android.util.Log;

import com.cnk.notificators.Notificator;
import com.cnk.utilities.Util;

import org.apache.thrift.TException;
import org.apache.thrift.protocol.TBinaryProtocol;
import org.apache.thrift.protocol.TProtocol;
import org.apache.thrift.transport.TSocket;
import org.apache.thrift.transport.TTransport;

public class RaportUploader extends ServerConnector {
    private static final String LOG_TAG = "ReportUploader";
    private static Notificator notificator;

    public static void setNotificator(Notificator n) {
        notificator = n;
    }

    @Override
    protected void onHandleIntent(Intent intent) {
        if (intent != null) {
            //TODO extract data from intent
            int tries = 0;
            sendData(intent.getIntExtra("tries", tries));
        }
    }

    private void sendData(int tries) {
        Log.i(LOG_TAG, "sending");
        tries = openSocket(tries);
        boolean succeeded = false;
        TProtocol protocol = new TBinaryProtocol(socket);
        Server.Client client = new Server.Client(protocol);

        HelloMsg msg = new HelloMsg();
        msg.msg = "test";
        msg.num = 123;

        while (!succeeded && tries > 0) {
            try {
                client.ping(msg);
                succeeded = true;
                Log.i(LOG_TAG, "sent");
            } catch (TException e1) {
                tries--;
                Log.e(LOG_TAG, "sending failed");
                Util.waitDelay(DELAY);
            }
        }
        if (tries <= 0) {
            notificator.failure();
        }
        socket.close();
        Log.i(LOG_TAG, "exiting");
    }

}
