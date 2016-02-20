package com.cnk.communication.task;

import android.util.Log;

import com.cnk.communication.thrift.Exhibit;
import com.cnk.communication.thrift.NewExhibitsRequest;
import com.cnk.communication.thrift.NewExhibitsResponse;
import com.cnk.communication.thrift.Server;
import com.cnk.data.DataHandler;
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
        NewExhibitsRequest request = new NewExhibitsRequest();
        Integer version = DataHandler.getInstance().getExhibitsVersion();
        if (version != null) {
            request.setAcquiredVersion(version);
        }
        NewExhibitsResponse response = client.getNewExhibits(request);
        updateDataHandler(response);
        Log.i(LOG_TAG, "Exhibits downloaded");
    }

    private void updateDataHandler(NewExhibitsResponse response) {
        Integer version = response.getVersion();
        Map<Integer, Exhibit> exhibits = response.getExhibits();

        List<com.cnk.database.models.Exhibit> dbExhibits = new ArrayList<>();
        for (Map.Entry<Integer, Exhibit> entry : exhibits.entrySet()) {
            dbExhibits.add(new com.cnk.database.models.Exhibit(entry.getKey(), entry.getValue()));
        }
        DataHandler.getInstance().setExhibits(dbExhibits, version);
    }

}
