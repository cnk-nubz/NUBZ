package com.cnk.communication.task;

import android.util.Log;

import com.cnk.communication.Exhibit;
import com.cnk.communication.ExhibitsRequest;
import com.cnk.communication.ExhibitsResponse;
import com.cnk.communication.Server;
import com.cnk.data.DataHandler;
import com.cnk.exceptions.InternalDatabaseError;
import com.cnk.notificators.Notificator;

import org.apache.thrift.TException;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;

public class ExhibitDownloadTask extends ServerTask {

    private static final String LOG_TAG = "ExhibitDownloadTask";

    public ExhibitDownloadTask(Notificator notificator) {
        super(notificator);
    }

    protected void performInSession(Server.Client client) throws TException {
        Log.i(LOG_TAG, "Downloading exhibits");
        ExhibitsRequest request = new ExhibitsRequest();
        /*
        if (DataHandler.getInstance().getExhibitsVersion() != null) {
            request.setAcquiredVersion(DataHandler.getInstance().getExhibitsVersion());
        }
        */
        ExhibitsResponse response = client.getExhibits(request);
        updateDataHandler(response);
        Log.i(LOG_TAG, "Exhibits downloaded");
    }

    private void updateDataHandler(ExhibitsResponse response) {

    }


}
