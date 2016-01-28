package com.cnk.communication.task;

import com.cnk.communication.Server;
import com.cnk.notificators.Notificator;

import org.apache.thrift.TException;

import java.io.IOException;

public class BackgroundDownloadTask extends ServerTask {

    public BackgroundDownloadTask(Notificator notificator) {
        super(notificator);
    }

    protected void performInSession(Server.Client client) throws TException, IOException {
        //ServerTask mapTask = new MapDownloadTask(notificator);
        ServerTask exhibitsTask = new ExhibitDownloadTask(notificator);
        //mapTask.performInSession(client);
        exhibitsTask.performInSession(client);
    }
}
