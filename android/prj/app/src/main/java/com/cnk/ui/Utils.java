package com.cnk.ui;

import android.app.Activity;
import android.content.DialogInterface;
import android.support.v7.app.AlertDialog;

import com.cnk.Cnk;

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
        String message = Cnk.getAppContext().getString(messageId);
        String positiveMessage = Cnk.getAppContext().getString(positiveId);
        String negativeMessage = null;
        if (negativeId != null) {
            negativeMessage = Cnk.getAppContext().getString(negativeId);
        }
        showDialog(a, message, positiveMessage, negativeMessage, positiveAction, negativeAction);
    }

    public static void showDialog(Activity a,
                                  String message,
                                  String positiveMessage,
                                  String negativeMessage,
                                  DialogInterface.OnClickListener positiveAction,
                                  DialogInterface.OnClickListener negativeAction) {
        AlertDialog.Builder alert = new AlertDialog.Builder(a);
        alert.setMessage(message);
        alert.setPositiveButton(positiveMessage, positiveAction);
        if (negativeMessage != null) {
            alert.setNegativeButton(negativeMessage, negativeAction);
        }
        alert.setCancelable(false);
        alert.show();
    }
}
