package com.cnk.communication.task;

import android.util.Log;

import com.cnk.communication.thrift.CurrentExperimentResponse;
import com.cnk.communication.thrift.Server;
import com.cnk.data.Action;
import com.cnk.data.DataHandler;
import com.cnk.notificators.Notificator;

import org.apache.thrift.TException;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

public class ExperimentDataDownloadTask extends ServerTask {

    private static final String LOG_TAG = "ExperimentDownloadTask";

    public ExperimentDataDownloadTask(Notificator notificator) {
        super(notificator);
    }

    @Override
    protected void performInSession(Server.Client client) throws TException, IOException {
        Log.i(LOG_TAG, "Downloading experiment");
        CurrentExperimentResponse thriftData = client.getCurrentExperiment();
        if (thriftData == null) {
            Log.i(LOG_TAG, "No active experiment, NullPointerException is coming");
        }
        Log.i(LOG_TAG, "Downloaded experiment");
        updateDataHandler(thriftData);
    }

    private void updateDataHandler(CurrentExperimentResponse thriftData) {
        com.cnk.data.Experiment experiment = translateDataFromThrift(thriftData);
        DataHandler.getInstance().setNewExperimentData(experiment);
    }

    private com.cnk.data.Experiment translateDataFromThrift(CurrentExperimentResponse thriftData) {
        Integer id = thriftData.getExperiment().getExperimentId();
        String name = thriftData.getExperiment().getName();
        List<Action> exhibitActions = translateActionsFromThrift(thriftData.getExperiment().getExhibitActions());
        List<Action> breakActions = translateActionsFromThrift(thriftData.getExperiment().getBreakActions());
        return new com.cnk.data.Experiment(id, name, exhibitActions, breakActions);
    }

    private List<Action> translateActionsFromThrift(List<com.cnk.communication.thrift.Action> thriftActions) {
        List<Action> actions = new ArrayList<>();
        for (com.cnk.communication.thrift.Action a : thriftActions) {
            actions.add(new Action(a.getActionId(), a.getText()));
        }
        return actions;
    }
}
