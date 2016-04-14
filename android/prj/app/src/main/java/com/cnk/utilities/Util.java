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
        return Cnk.getAppContext().getString(R.string.dataDownloadFailure) + " - " +
               Cnk.getAppContext()
                  .getString(reason == ServerTask.FailureReason.NOWIFI ? R.string.reasonWifi :
                                     R.string.reasonConnection);
    }


}
