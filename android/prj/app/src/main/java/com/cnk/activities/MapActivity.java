package com.cnk.activities;

import android.content.Intent;
import android.os.Bundle;
import android.support.annotation.NonNull;
import android.support.v7.app.ActionBar;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.util.Log;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Chronometer;
import android.widget.RelativeLayout;
import android.widget.TextView;

import com.cnk.R;
import com.cnk.activities.map.ActionsDialog;
import com.cnk.activities.map.MapContainer;
import com.cnk.data.exhibits.ExhibitsData;
import com.cnk.data.experiment.Action;
import com.cnk.data.experiment.ExperimentData;
import com.cnk.data.experiment.survey.Survey;
import com.cnk.data.raports.RaportEvent;
import com.cnk.database.models.Exhibit;
import com.cnk.notificators.Observer;
import com.cnk.ui.Utils;
import com.cnk.utilities.Consts;

import java.util.ArrayList;
import java.util.List;

public class MapActivity extends AppCompatActivity implements Observer {
    private static final String LOG_TAG = "MapActivity";

    private boolean dialogOpened;
    private ActionsDialog exhibitDialog;
    private ActionsDialog breakDialog;

    private TextView floorTitle;
    private int currentFloor;
    private MapContainer mapContainer;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        prepareViews();
        prepareDialogs();

        setFloor(0);

        ExhibitsData.getInstance().addObserver(this, this::onExhibitsChange);
        mapContainer.addObserver(this, this::exhibitClick);
    }

    @Override
    protected void onPostCreate(Bundle savedInstanceState) {
        super.onPostCreate(savedInstanceState);
    }

    private void prepareViews() {
        setContentView(R.layout.map_activity_layout);
        RelativeLayout rootLayout = (RelativeLayout) findViewById(R.id.rlRootViewMapActivity);

        prepareMapContainer(rootLayout);
        prepareActionBar();
    }

    private void prepareMapContainer(@NonNull RelativeLayout parentLayout) {
        List<Exhibit> exhibits = new ArrayList<>();
        for (int i = 0; i < Consts.FLOOR_COUNT; i++) {
            exhibits.addAll(ExhibitsData.getInstance().getExhibitsOfFloor(i));
        }
        mapContainer = new MapContainer(MapActivity.this, parentLayout);
        mapContainer.updateExhibits(exhibits, true);
    }

    private void prepareActionBar() {
        ActionBar actionBar = getSupportActionBar();
        if (actionBar != null) {
            actionBar.setDisplayShowCustomEnabled(true);
            actionBar.setDisplayShowTitleEnabled(false);
            actionBar.setDisplayOptions(ActionBar.DISPLAY_SHOW_CUSTOM);
            View abView = getLayoutInflater().inflate(R.layout.map_activity_bar, null);
            Toolbar.LayoutParams abParams =
                    new Toolbar.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT,
                                             ViewGroup.LayoutParams.MATCH_PARENT);
            actionBar.setCustomView(abView, abParams);
            Toolbar parent = (Toolbar) abView.getParent();
            parent.setContentInsetsAbsolute(0, 0);

            abView.findViewById(R.id.finish_button).setOnClickListener(this::endClick);
            ((Chronometer) abView.findViewById(R.id.chrono_experiment_time)).start();
            floorTitle = (TextView) abView.findViewById(R.id.floor_name);
            floorTitle.setText(getString(R.string.floor) + " 1");
            abView.findViewById(R.id.break_button).setOnClickListener(this::pauseClick);
            abView.findViewById(R.id.change_floor_button)
                  .setOnClickListener((View v) -> setFloor((currentFloor + 1) %
                                                           Consts.FLOOR_COUNT));
        }
    }

    private void prepareDialogs() {
        exhibitDialog = createActionsDialog(ExperimentData.getInstance().getAllExhibitActions());

        breakDialog = createActionsDialog(ExperimentData.getInstance().getAllBreakActions());
        breakDialog.setTitle(getString(R.string.break_action));
    }

    private void onExhibitsChange(@NonNull List<Exhibit> changedExhibits, boolean fullRefresh) {
        runOnUiThread(() -> mapContainer.updateExhibits(changedExhibits, fullRefresh));
    }

    private ActionsDialog createActionsDialog(@NonNull List<Action> actions) {
        View dialogView = this.getLayoutInflater().inflate(R.layout.exhibit_dialog_layout, null);
        return ActionsDialog.create(MapActivity.this, dialogView, actions);
    }

    public void pauseClick(View view) {
        Log.i(LOG_TAG, "Break button clicked");
        if (!dialogOpened) {
            dialogOpened = true;
            breakDialog.reset();
            breakDialog.show(this::dialogCanceled, () -> dialogFinished(null, breakDialog));
        }
    }

    private void exhibitClick(@NonNull Exhibit exhibit) {
        Log.i(LOG_TAG, "Exhibit " + exhibit.getId() + " clicked");
        if (!dialogOpened) {
            dialogOpened = true;
            exhibitDialog.reset();
            exhibitDialog.setTitle(exhibit.getName());
            exhibitDialog.show(this::dialogCanceled,
                               () -> dialogFinished(exhibit.getId(), exhibitDialog));
        }
    }

    private void dialogFinished(Integer exhibitId, @NonNull ActionsDialog dialog) {
        List<Integer> actions = dialog.getSelectedActions();
        Log.i(LOG_TAG, "Selected actions: " + actions);
        RaportEvent event =
                new RaportEvent(exhibitId, dialog.getElapsedTime(), actions, dialog.getBeginDate());
        ExperimentData.getInstance().addEventToCurrentRaportInBg(event);

        dialogOpened = false;
    }

    private void dialogCanceled() {
        dialogOpened = false;
    }

    public void endClick(View view) {
        Log.i(LOG_TAG, "End button clicked");
        Utils.showDialog(this,
                         R.string.confirm_map_close,
                         R.string.confirm,
                         R.string.cancel,
                         (dialog, which) -> {
                             Log.i(LOG_TAG, "End map confirmed");
                             Intent postSurvey =
                                     new Intent(getApplicationContext(), SurveyActivity.class);
                             postSurvey.putExtra("type", Survey.SurveyType.AFTER);
                             startActivity(postSurvey);
                             finish();
                         });
    }

    private void setFloor(int floor) {
        floorTitle.setText(getString(R.string.floor) + " " + floor);
        mapContainer.switchToFloor(floor);
        currentFloor = floor;
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        ExhibitsData.getInstance().deleteObserver(this);
        mapContainer.destroy();
    }

    @Override
    public void onBackPressed() {
        // overriden to stop back button from working
    }
}
