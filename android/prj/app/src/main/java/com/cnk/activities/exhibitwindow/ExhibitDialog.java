package com.cnk.activities.exhibitwindow;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.GridView;
import android.widget.ProgressBar;

import com.cnk.R;
import com.cnk.data.experiment.Action;
import com.cnk.data.DataHandler;
import com.cnk.ui.AutoResizeTextView;
import com.cnk.utilities.Consts;

import java.util.ArrayList;
import java.util.List;

public class ExhibitDialog extends Activity {
    public static final String IS_BREAK = "IsBreak";
    public static final String NAME = "Name";
    public static final String SELECTED_ACTIONS = "Selected actions";
    public static final String TIME = "Time";

    private List<String> actionsStrings;
    private String name;
    private Boolean isBreak;
    private ExhibitActionsAdapter actionsAdapter;
    private long startTime;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        startTime = System.currentTimeMillis() / Consts.SECOND;
        setFinishOnTouchOutside(false);

        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);

        name = getIntent().getStringExtra(NAME);
        isBreak = getIntent().getBooleanExtra(IS_BREAK, false);

        setContentView(R.layout.exhibit_dialog_layout);

        actionsStrings = new ArrayList<>();

        List<Action> actions;
        if (isBreak) {
            actions = DataHandler.getInstance().getAllBreakActions();
        } else {
            actions = DataHandler.getInstance().getAllExhibitActions();
        }
        for (Action a : actions) {
            actionsStrings.add(a.getText());
        }
        actionsAdapter = new ExhibitActionsAdapter(ExhibitDialog.this, actionsStrings);

        Button cancel = (Button) findViewById(R.id.bExbibitDialogCancel);
        cancel.setOnClickListener(new CancelListener());

        Button finish = (Button) findViewById(R.id.bExbibitDialogFinish);
        finish.setOnClickListener(new FinishListener());

        AutoResizeTextView tvExhibitDialogName = (AutoResizeTextView) findViewById(R.id.tvExhibitDialogName);
        tvExhibitDialogName.setText(name);
        tvExhibitDialogName.setMinTextSize(2f);
        tvExhibitDialogName.setTextSize(100f);

        GridView gridView = (GridView) findViewById(R.id.gvActions);
        gridView.setAdapter(actionsAdapter);

        ProgressBar progressBar = (ProgressBar) findViewById(R.id.pbExhibitDialog);
        progressBar.setVisibility(View.INVISIBLE);

        gridView.setVisibility(View.VISIBLE);
    }

    @Override
    public void onBackPressed() {
    }


    class CancelListener implements View.OnClickListener {
        @Override
        public void onClick(View view) {
            setResult(RESULT_CANCELED);
            finish();
        }
    }

    class FinishListener implements View.OnClickListener {
        @Override
        public void onClick(View view) {
            long endTime = System.currentTimeMillis() / Consts.SECOND;
            Intent intent = new Intent();
            intent.putExtra(TIME, endTime - startTime);
            intent.putStringArrayListExtra(SELECTED_ACTIONS, actionsAdapter.getSelectedActions());
            setResult(RESULT_OK, intent);
            finish();
        }
    }
}
