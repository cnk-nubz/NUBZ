package com.cnk.activities;

import android.content.Intent;
import android.os.Bundle;
import android.support.annotation.NonNull;
import android.support.v4.widget.DrawerLayout;
import android.support.v7.app.ActionBarDrawerToggle;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.widget.Chronometer;
import android.widget.RelativeLayout;

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
import com.cnk.utilities.Consts;

import java.util.ArrayList;
import java.util.List;

public class MapActivity extends AppCompatActivity implements Observer {
    private static final String LOG_TAG = "MapActivity";

    private ActionBarDrawerToggle drawerToggle;

    private boolean dialogOpened;
    private ActionsDialog exhibitDialog;
    private ActionsDialog breakDialog;

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
        drawerToggle.syncState();
    }

    private void prepareViews() {
        setContentView(R.layout.map_activity_layout);
        RelativeLayout rootLayout = (RelativeLayout) findViewById(R.id.rlRootViewMapActivity);

        prepareMapContainer(rootLayout);
        prepareActionBar();
        prepareDrawer();
    }

    private void prepareMapContainer(@NonNull RelativeLayout parentLayout) {
        List<Exhibit> exhibits = new ArrayList<>();
        for (int i = 0; i < Consts.FLOOR_COUNT; i++) {
            exhibits.addAll(ExhibitsData.getInstance().getExhibitsOfFloor(i));
        }
        mapContainer = new MapContainer(MapActivity.this, parentLayout);
        mapContainer.updateExhibits(exhibits);
    }

    private void prepareActionBar() {
        DrawerLayout drawerLayout = (DrawerLayout) findViewById(R.id.drawer_layout);
        drawerToggle = new ActionBarDrawerToggle(this,
                                                 drawerLayout,
                                                 R.string.drawer_open,
                                                 R.string.drawer_close) {

            public void onDrawerClosed(View view) {
                super.onDrawerClosed(view);
            }

            public void onDrawerOpened(View drawerView) {
                super.onDrawerOpened(drawerView);
            }
        };
        if (getSupportActionBar() != null) {
            getSupportActionBar().setDisplayHomeAsUpEnabled(true);
            getSupportActionBar().setHomeButtonEnabled(true);
        }
        drawerLayout.setDrawerListener(drawerToggle);
    }

    private void prepareDrawer() {
        ((Chronometer) findViewById(R.id.chronometer)).start();
    }

    private void prepareDialogs() {
        exhibitDialog = createActionsDialog(ExperimentData.getInstance().getAllExhibitActions());

        breakDialog = createActionsDialog(ExperimentData.getInstance().getAllBreakActions());
        breakDialog.setTitle(getString(R.string.break_action));
    }

    private void onExhibitsChange(@NonNull List<Exhibit> changedExhibits) {
        runOnUiThread(() -> mapContainer.updateExhibits(changedExhibits));
    }

    private ActionsDialog createActionsDialog(@NonNull List<Action> actions) {
        View dialogView = this.getLayoutInflater().inflate(R.layout.exhibit_dialog_layout, null);
        return ActionsDialog.create(MapActivity.this, dialogView, actions);
    }

    @SuppressWarnings("unused")
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

    @SuppressWarnings("unused")
    public void endClick(View view) {
        Log.i(LOG_TAG, "End button clicked");

        Intent postSurvey = new Intent(getApplicationContext(), SurveyActivity.class);
        postSurvey.putExtra("type", Survey.SurveyType.AFTER);
        startActivity(postSurvey);
        finish();
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        MenuInflater inflater = getMenuInflater();
        inflater.inflate(R.menu.map_activity_menu, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        if (drawerToggle.onOptionsItemSelected(item)) {
            return true;
        } else if (item.getItemId() == R.id.action_next_floor) {
            setFloor((currentFloor + 1) % Consts.FLOOR_COUNT);
        }
        return super.onOptionsItemSelected(item);
    }

    private void setFloor(int floor) {
        setTitle(getString(R.string.pietro) + " " + floor);
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
