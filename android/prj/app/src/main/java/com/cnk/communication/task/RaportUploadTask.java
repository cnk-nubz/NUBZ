package com.cnk.communication.task;

import android.util.Log;

import com.cnk.communication.HelloMsg;
import com.cnk.communication.Server;
import com.cnk.notificators.Notificator;

import org.apache.thrift.TException;

import java.io.File;

public class RaportUploadTask extends ServerTask {

    private static final String LOG_TAG = "RaportUploadTask";
    private File toSend;

    public RaportUploadTask(Notificator notificator, File toSend) {
        super(notificator);
        this.toSend = toSend;
    }

    @Override
    protected void performInSession(Server.Client client) {
        Log.i(LOG_TAG, "Uploading raport");
        HelloMsg msg = new HelloMsg();
        msg.msg = "test";
        msg.num = 123;
        try {
            client.ping(msg);
        } catch (TException e) {
            Log.e(LOG_TAG, "Error");
            e.printStackTrace();
        }
        Log.i(LOG_TAG, "Raport uploaded");
    }

}
