package com.cnk.ui;

import android.app.Activity;
import android.content.DialogInterface;
import android.support.v7.app.AlertDialog;

public class Utils {
    public static void showDialog(Activity a,
                                  int messageId,
                                  int positiveId,
                                  Integer negativeId,
                                  DialogInterface.OnClickListener positiveAction) {
        showDialog(a, messageId, positiveId, negativeId, positiveAction, null);
    }

    public static void showDialog(Activity a,
                                  int messageId,
                                  int positiveId,
                                  Integer negativeId,
                                  DialogInterface.OnClickListener positiveAction,
                                  DialogInterface.OnClickListener negativeAction) {
        AlertDialog.Builder alert = new AlertDialog.Builder(a);
        alert.setMessage(messageId);
        alert.setPositiveButton(positiveId, positiveAction);
        if (negativeId != null) {
            alert.setNegativeButton(negativeId, negativeAction);
        }
        alert.setCancelable(false);
        alert.show();
    }
}
