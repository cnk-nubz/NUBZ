package com.cnk.utilities;

import com.cnk.Cnk;
import com.cnk.R;
import com.cnk.communication.task.ServerTask;

public class Util {

    public static void waitDelay(long delay) {
        try {
            Thread.sleep(delay * Consts.MILLIS_IN_SEC);
        } catch (java.lang.InterruptedException e2) {
            e2.printStackTrace();
        }
    }

    public static String downloadErrorMessage(ServerTask.FailureReason reason) {
        if (reason == ServerTask.FailureReason.NOWIFI) {
            return Cnk.getAppContext().getString(R.string.dataDownloadFailure) + " - " +
                   Cnk.getAppContext().getString(R.string.reasonWifi);
        } else if (reason == ServerTask.FailureReason.NO_EXPERIMENT) {
            return Cnk.getAppContext().getString(R.string.dataDownloadFailure) + " - " +
                   Cnk.getAppContext().getString(R.string.reasonNoExperiment);
        } else {
            return Cnk.getAppContext().getString(R.string.dataDownloadFailure) + " - " +
                   Cnk.getAppContext().getString(R.string.reasonConnection);
        }
    }
}
