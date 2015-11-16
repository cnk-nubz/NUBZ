package com.cnk.communication.task;

import android.util.Log;

import com.cnk.communication.HelloMsg;
import com.cnk.communication.Server;
import com.cnk.notificators.Notificator;
import com.cnk.utilities.Util;

import org.apache.thrift.TException;
import org.apache.thrift.protocol.TBinaryProtocol;
import org.apache.thrift.protocol.TProtocol;

import java.io.File;

public class RaportUploadTask extends ServerTask {

    private static final String LOG_TAG = "RaportUploadTask";
    private File toSend;

    public RaportUploadTask(Notificator notificator, File toSend) {
        super(notificator);
        this.toSend = toSend;
    }

    public void run(int tries) {
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
        } else {
            notificator.success();
        }
        socket.close();
        Log.i(LOG_TAG, "exiting");
    }

}
