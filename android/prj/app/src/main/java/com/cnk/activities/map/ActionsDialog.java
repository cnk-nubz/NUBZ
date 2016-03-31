package com.cnk.activities.map;

import android.app.Dialog;
import android.content.Context;
import android.os.SystemClock;
import android.support.annotation.NonNull;
import android.support.v7.app.AlertDialog;
import android.view.View;
import android.widget.Button;
import android.widget.Chronometer;
import android.widget.GridView;
import android.widget.TextView;

import com.cnk.R;
import com.cnk.data.experiment.Action;
import com.cnk.ui.AutoResizeTextView;
import com.cnk.ui.adapters.SelectListAdapter;
import com.cnk.utilities.Consts;

import java.util.Date;
import java.util.List;

public class ActionsDialog {
    public interface Observer {
        void onAction();
    }

    private long beginTime;
    private long endTime;
    
    private SelectListAdapter<Action> adapter;

    private Dialog dialog;
    private Button cancelButton;
    private Button finishButton;
    private Chronometer chronometer;
    private TextView textView;

    public static ActionsDialog create(@NonNull Context context,
                                       @NonNull View view,
                                       @NonNull List<Action> actions) {
        AlertDialog.Builder
                dialogBuilder =
                new AlertDialog.Builder(context, R.style.FullHeightDialog);
        dialogBuilder.setView(view);
        dialogBuilder.setCancelable(false);

        AutoResizeTextView
                textView =
                (AutoResizeTextView) view.findViewById(R.id.tvExhibitDialogName);
        textView.setMinTextSize(2f);
        textView.setTextSize(100f);

        ActionsDialog actionsDialog = new ActionsDialog();
        actionsDialog.dialog = dialogBuilder.create();
        actionsDialog.chronometer = (Chronometer) view.findViewById(R.id.chronometer);
        actionsDialog.cancelButton = (Button) view.findViewById(R.id.bExbibitDialogCancel);
        actionsDialog.finishButton = (Button) view.findViewById(R.id.bExbibitDialogFinish);
        actionsDialog.textView = textView;
        actionsDialog.adapter = new SelectListAdapter<>(actions, false, context);

        actionsDialog.dialog.getWindow().getAttributes().windowAnimations = R.style.DialogAnimation;

        GridView breakGridView = (GridView) view.findViewById(R.id.gvActions);
        breakGridView.setAdapter(actionsDialog.adapter);

        return actionsDialog;
    }

    private ActionsDialog() {

    }

    public void setTitle(String title) {
        textView.setText(title);
    }

    public void reset() {
        chronometer.setBase(SystemClock.elapsedRealtime());
        adapter.unselectAllOptions();
    }

    public void show(Observer cancelObserver, Observer finishObserver) {
        cancelButton.setOnClickListener((view) -> {
            dismiss();
            cancelObserver.onAction();
        });
        finishButton.setOnClickListener((view) -> {
            dismiss();
            finishObserver.onAction();
        });

        beginTime = new Date().getTime();
        chronometer.start();
        dialog.show();
    }

    private void dismiss() {
        endTime = new Date().getTime();
        chronometer.stop();
        cancelButton.setOnClickListener(null);
        finishButton.setOnClickListener(null);
        dialog.dismiss();
    }

    public List<Integer> getSelectedActions() {
        return adapter.getSelectedOptions();
    }

    // in seconds
    public int getElapsedTime() {
        return (int) ((getEndTime() - getBeginTime()) / Consts.MILLIS_IN_SEC);
    }

    public long getBeginTime() {
        return beginTime;
    }

    public long getEndTime() {
        return endTime;
    }

    public Date getBeginDate() {
        return new Date(beginTime);
    }
}
