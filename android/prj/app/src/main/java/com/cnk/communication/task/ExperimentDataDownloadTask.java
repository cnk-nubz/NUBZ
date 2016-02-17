package com.cnk.communication.task;

import android.util.Log;

import com.cnk.communication.ExperimentData;
import com.cnk.communication.Server;
import com.cnk.data.Action;
import com.cnk.data.DataHandler;
import com.cnk.notificators.Notificator;

import org.apache.thrift.TException;

import java.util.ArrayList;
import java.util.List;

public class ExperimentDataDownloadTask extends ServerTask {

    private static final String LOG_TAG = "ExperimentDownloadTask";

    public ExperimentDataDownloadTask(Notificator notificator) {
        super(notificator);
    }

    @Override
    protected void performInSession(Server.Client client) throws TException {
        Log.i(LOG_TAG, "Downloading actions");
        ExperimentData thriftData = client.getExperimentData();
        Log.i(LOG_TAG, "Downloaded actions");
        updateDataHandler(thriftData);
    }

    private com.cnk.data.ExperimentData translateDataFromThrift(ExperimentData thriftData) {
        List<Action> exhibitActions = translateActionsFromThrift(thriftData.getExhibitActions());
        List<Action> breakActions = translateActionsFromThrift(thriftData.getBreakActions());
        return new com.cnk.data.ExperimentData(exhibitActions, breakActions);
    }

    private List<Action> translateActionsFromThrift(List<com.cnk.communication.Action> thriftActions) {
        List<Action> actions = new ArrayList<>();
        for (com.cnk.communication.Action a : thriftActions) {
            actions.add(new Action(a.getActionId(), a.getText()));
        }
        return actions;
    }

    private void updateDataHandler(ExperimentData thriftData) {
        com.cnk.data.ExperimentData experimentData = translateDataFromThrift(thriftData);
        DataHandler.getInstance().setNewExperimentData(experimentData);
    }
}
