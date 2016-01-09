package com.cnk.communication.task;

import android.util.Log;

import com.cnk.communication.ExperimentData;
import com.cnk.communication.Server;
import com.cnk.data.Action;
import com.cnk.data.DataHandler;
import com.cnk.notificators.Notificator;

import java.util.ArrayList;
import java.util.List;

public class ExperimantDataDownloadTask extends ServerTask {

    private static final String LOG_TAG = "ExperimantDataDownloadTask";

    public ExperimantDataDownloadTask(Notificator notificator) {
        super(notificator);
    }

    @Override
    protected void performInSession(Server.Client client) throws Exception {
        Log.i(LOG_TAG, "Downloading actions");
        ExperimentData thriftData = client.getExperimentData();
        Log.i(LOG_TAG, "Downloaded actions");
        com.cnk.data.ExperimentData experimentData = translateDataFromThrift(thriftData);
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

    private void updateDataHandler(com.cnk.data.ExperimentData experimentData) {
        DataHandler.getInstance().setNewExperimentData(experimentData);
    }
}
