package com.cnk.communication.task;

import android.util.Log;

import com.cnk.communication.Experiment;
import com.cnk.communication.Server;
import com.cnk.data.Action;
import com.cnk.data.DataHandler;
import com.cnk.notificators.Notificator;

import java.util.ArrayList;
import java.util.List;

public class ExperimentDataDownloadTask extends ServerTask {

    private static final String LOG_TAG = "ExperimentDownloadTask";

    public ExperimentDataDownloadTask(Notificator notificator) {
        super(notificator);
    }

    @Override
    protected void performInSession(Server.Client client) throws Exception {
        Log.i(LOG_TAG, "Downloading experiment");
        Experiment thriftData = client.getCurrentExperiment().experiment;
        if (thriftData == null) {
            Log.i(LOG_TAG, "No active experiment, NullPointerException is coming");
        }
        Log.i(LOG_TAG, "Downloaded experiment");
        updateDataHandler(thriftData);
    }

    private void updateDataHandler(Experiment thriftData) {
        com.cnk.data.Experiment experiment = translateDataFromThrift(thriftData);
        DataHandler.getInstance().setNewExperimentData(experiment);
    }

    private com.cnk.data.Experiment translateDataFromThrift(Experiment thriftData) {
        Integer id = thriftData.experimentId;
        String name = thriftData.name;
        List<Action> exhibitActions = translateActionsFromThrift(thriftData.getExhibitActions());
        List<Action> breakActions = translateActionsFromThrift(thriftData.getBreakActions());
        return new com.cnk.data.Experiment(id, name, exhibitActions, breakActions);
    }

    private List<Action> translateActionsFromThrift(List<com.cnk.communication.Action> thriftActions) {
        List<Action> actions = new ArrayList<>();
        for (com.cnk.communication.Action a : thriftActions) {
            actions.add(new Action(a.getActionId(), a.getText()));
        }
        return actions;
    }
}
