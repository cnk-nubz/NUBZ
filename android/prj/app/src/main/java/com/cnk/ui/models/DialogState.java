package com.cnk.ui.models;

import android.widget.Button;
import android.widget.Chronometer;

import com.cnk.data.experiment.Action;
import com.cnk.ui.AutoResizeTextView;
import com.cnk.ui.adapters.SelectListAdapter;

import java.util.Date;

public class DialogState {
    private Long dialogOpenedTime;
    private android.support.v7.app.AlertDialog dialog;
    private AutoResizeTextView tvDialogName;
    private SelectListAdapter<Action> adapter;
    private Button cancelButton;
    private Button finishButton;
    private Chronometer timer;

    public android.support.v7.app.AlertDialog getDialog() {
        return dialog;
    }

    public void setDialog(android.support.v7.app.AlertDialog dialog) {
        this.dialog = dialog;
    }

    public AutoResizeTextView getTvDialogName() {
        return tvDialogName;
    }

    public void setTvDialogName(AutoResizeTextView tvDialogName) {
        this.tvDialogName = tvDialogName;
    }

    public SelectListAdapter<Action> getAdapter() {
        return adapter;
    }

    public void setAdapter(SelectListAdapter<Action> adapter) {
        this.adapter = adapter;
    }

    public Button getCancelButton() {
        return cancelButton;
    }

    public void setCancelButton(Button cancelButton) {
        this.cancelButton = cancelButton;
    }

    public Button getFinishButton() {
        return finishButton;
    }

    public void setFinishButton(Button finishButton) {
        this.finishButton = finishButton;
    }

    public void showDialog() {
        this.dialogOpenedTime = new Date().getTime();
        this.dialog.show();
    }

    public Integer dismissDialogWithTime() {
        dialog.dismiss();
        return ((Long) (new Date().getTime() - dialogOpenedTime)).intValue();
    }

    public Chronometer getTimer() {
        return timer;
    }

    public void setTimer(Chronometer timer) {
        this.timer = timer;
    }
}
