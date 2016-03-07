package com.cnk.ui.models;

import android.app.AlertDialog;
import android.widget.Button;

import com.cnk.ui.AutoResizeTextView;
import com.cnk.ui.exhibitwindow.ExhibitActionsAdapter;

import java.util.Date;

public class DialogState {
    private Long dialogOpenedTime;
    private android.support.v7.app.AlertDialog dialog;
    private AutoResizeTextView tvDialogName;
    private ExhibitActionsAdapter adapter;
    private Button cancelButton;
    private Button finishButton;

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

    public ExhibitActionsAdapter getAdapter() {
        return adapter;
    }

    public void setAdapter(ExhibitActionsAdapter adapter) {
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
}
