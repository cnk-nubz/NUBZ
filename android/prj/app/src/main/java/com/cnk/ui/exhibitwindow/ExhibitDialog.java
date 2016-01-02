package com.cnk.ui.exhibitwindow;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.GridView;
import android.widget.TextView;

import com.cnk.R;

import java.util.List;

public class ExhibitDialog extends Activity {
    private Integer reqId;
    private List<String> actions;
    private String name;
    private ExhibitActionsAdapter actionsAdapter;

    public static final String NAME = "Name";
    public static final String AVAILABLE_ACTIONS = "Available actions";
    public static final String SELECTED_ACTIONS = "Selected actions";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setFinishOnTouchOutside(false);

        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);

        name = getIntent().getStringExtra(NAME);
        actions = getIntent().getStringArrayListExtra(AVAILABLE_ACTIONS);

        setContentView(R.layout.exhibit_dialog_layout);
        Button cancel = (Button) findViewById(R.id.bExbibitDialogCancel);
        cancel.setOnClickListener(new CancelListener());

        TextView tvExhibitDialogName = (TextView) findViewById(R.id.tvExhibitDialogName);
        tvExhibitDialogName.setText(name);
        tvExhibitDialogName.setTextAppearance(this, android.R.style.TextAppearance_Large);

        Button finish = (Button) findViewById(R.id.bExbibitDialogFinish);
        finish.setOnClickListener(new FinishListener());

        GridView gridView = (GridView) findViewById(R.id.gvActions);
        actionsAdapter = new ExhibitActionsAdapter(this, actions);
        gridView.setAdapter(actionsAdapter);
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
            Intent intent = new Intent();
            intent.putStringArrayListExtra(SELECTED_ACTIONS, actionsAdapter.getSelectedActions());
            setResult(RESULT_OK, intent);
            finish();
        }
    }
}
