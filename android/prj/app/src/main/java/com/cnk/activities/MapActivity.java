package com.cnk.activities;

import android.app.ProgressDialog;
import android.content.Context;
import android.content.Intent;
import android.graphics.Color;
import android.graphics.Rect;
import android.graphics.drawable.Drawable;
import android.graphics.drawable.GradientDrawable;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.SystemClock;
import android.support.v4.widget.DrawerLayout;
import android.support.v7.app.ActionBarDrawerToggle;
import android.support.v7.app.AlertDialog;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.util.Pair;
import android.view.Gravity;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.Chronometer;
import android.widget.GridView;
import android.widget.LinearLayout;
import android.widget.ProgressBar;
import android.widget.RelativeLayout;
import android.widget.TextView;

import com.cnk.R;
import com.cnk.communication.NetworkHandler;
import com.cnk.data.experiment.Action;
import com.cnk.database.models.DetailLevelRes;
import com.cnk.database.models.Exhibit;
import com.cnk.ui.adapters.SelectListAdapter;
import com.cnk.ui.models.DialogState;
import com.cnk.data.exhibits.ExhibitsData;
import com.cnk.data.experiment.ExperimentData;
import com.cnk.data.experiment.raport.RaportEvent;
import com.cnk.data.experiment.survey.Survey;
import com.cnk.data.map.MapData;
import com.cnk.data.map.Resolution;
import com.cnk.notificators.Observer;
import com.cnk.ui.AutoResizeTextView;
import com.cnk.ui.ImageHelper;
import com.cnk.ui.MapBitmapProvider;
import com.cnk.ui.ScaleData;
import com.cnk.ui.models.ExhibitSpot;
import com.cnk.ui.models.MapState;
import com.cnk.utilities.ColorHelper;
import com.cnk.utilities.Consts;
import com.qozix.tileview.TileView;
import com.qozix.tileview.hotspots.HotSpot;

import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;
import java.util.concurrent.Semaphore;

public class MapActivity extends AppCompatActivity implements Observer {
    private static final String LOG_TAG = "MapActivity";
    public static final Float MAXIMUM_SCALE = 4.0f;
    public static final Float MINIMUM_SCALE = 0.01f;

    private TileView tileView;
    private Semaphore changeAndUpdateMutex;
    private MapState mapState;
    private LinearLayout layoutLoading;
    private LinearLayout layoutMapMissing;
    private Integer currentFloorNum;
    private RelativeLayout rlRootLayout;
    private ProgressDialog spinner;
    private ActionBarDrawerToggle drawerToggle;
    private Integer openedDialogs;

    private DialogState exhibitDialog;
    private DialogState breakDialog;

    private class StartUpTask extends AsyncTask<Void, Void, Boolean> {
        @Override
        protected Boolean doInBackground(Void... voids) {
            if (MapData.getInstance().mapForFloorExists(currentFloorNum)) {
                return true;
            } else {
                setLayout(false);
                return false;
            }
        }

        @Override
        protected void onPostExecute(Boolean b) {
            super.onPostExecute(b);
            if (b) {
                new MapChangeFloor().execute();
            }
        }
    }

    // Map changing:
    private class MapRefreshExhibits extends AsyncTask<Void, Void, Void> {
        @Override
        protected Void doInBackground(Void... voids) {
            Log.i(LOG_TAG, "refreshing exhibits beginning");
            changeAndUpdateMutex.acquireUninterruptibly();

            final Semaphore waitForUIMutex = new Semaphore(0, true);

            runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    if (tileView != null) {
                        tileView.setVisibility(View.INVISIBLE);
                    }
                    if (layoutMapMissing != null) {
                        layoutMapMissing.setVisibility(View.INVISIBLE);
                    }
                    if (layoutLoading != null) {
                        layoutLoading.setVisibility(View.VISIBLE);
                    }
                    waitForUIMutex.release();
                }
            });

            clearAllExhibitsOnMap();
            waitForUIMutex.acquireUninterruptibly();
            addAllExhibitsToMap(ExhibitsData.getInstance().getExhibitsOfFloor(currentFloorNum));

            changeAndUpdateMutex.release();

            Log.i(LOG_TAG, "refreshing exhibits end");
            return null;
        }
    }

    private class MapChangeFloor extends AsyncTask<Void, Void, Void> {
        private Integer floor;

        @Override
        protected Void doInBackground(Void... voids) {
            Log.i(LOG_TAG, "map setting beginning");

            System.gc();

            final Semaphore localUISynchronization = new Semaphore(0, true);

            changeAndUpdateMutex.acquireUninterruptibly();

            floor = currentFloorNum;

            runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    if (layoutMapMissing != null) {
                        layoutMapMissing.setVisibility(View.INVISIBLE);
                    }
                    if (tileView != null) {
                        tileView.setVisibility(View.INVISIBLE);
                    }
                    if (layoutLoading != null) {
                        layoutLoading.setVisibility(View.VISIBLE);
                    }
                    localUISynchronization.release();
                }
            });

            localUISynchronization.acquireUninterruptibly();

            Integer detailLevels = MapData.getInstance().getDetailLevelsCountForFloor(floor);
            DetailLevelRes
                    biggestResolution =
                    MapData.getInstance().getDetailLevelResolution(floor, detailLevels - 1);

            mapState.currentMapSize = biggestResolution.getScaledRes();
            mapState.originalMapSize = MapData.getInstance().getOriginalResolution(floor);

            if (detailLevels == null || biggestResolution == null ||
                mapState.currentMapSize == null || mapState.originalMapSize == null) {
                showAlert();
            }

            LinkedList<ScaleData> ll = new LinkedList<>();
            for (int i = 0; i < detailLevels; i++) {
                DetailLevelRes current = MapData.getInstance().getDetailLevelResolution(floor, i);
                if (current == null) {
                    showAlert();
                }
                ll.add(new ScaleData((float) current.getScaledRes().getWidth() /
                                     biggestResolution.getScaledRes().getWidth(), i));
            }

            ArrayList<Resolution> tileSizes = new ArrayList<>();
            for (int i = 0; i < detailLevels; i++) {
                Resolution current = MapData.getInstance().getTileSize(floor, i);
                if (current == null) {
                    showAlert();
                }
                tileSizes.add(current);
            }

            prepareTileView(floor, ll, tileSizes);
            setLayout(true);

            addAllExhibitsToMap(ExhibitsData.getInstance().getExhibitsOfFloor(floor));

            changeAndUpdateMutex.release();

            Log.i(LOG_TAG, "map setting end");
            return null;
        }
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        Log.i(LOG_TAG, "onCreate execution");
        currentFloorNum = 0;
        changeAndUpdateMutex = new Semaphore(1, true);
        openedDialogs = 0;
        mapState = new MapState(this);

        Log.i(LOG_TAG, "adding to ExhibitsData observers list");
        ExhibitsData.getInstance().addObserver(this, this::onExhibitsChange);

        setViews();
        setActionBar();
        new StartUpTask().execute();
        breakDialog = createDialog(ExperimentData.getInstance().getAllBreakActions());
        exhibitDialog = createDialog(ExperimentData.getInstance().getAllExhibitActions());

        Log.i(LOG_TAG, "starting background download");
        NetworkHandler.getInstance().startBgDownload();
    }

    private void onExhibitsChange(List<Exhibit> changedExhibits) {
        Log.i(LOG_TAG, "Received exhibits update notification");
        new MapRefreshExhibits().execute();
    }

    private DialogState createDialog(List<Action> dialogActions) {
        DialogState resultDialog = new DialogState();

        AlertDialog.Builder
                dialogBuilder =
                new AlertDialog.Builder(MapActivity.this, R.style.FullHeightDialog);
        View dialogView = this.getLayoutInflater().inflate(R.layout.exhibit_dialog_layout, null);
        dialogBuilder.setView(dialogView);

        resultDialog.setTimer((Chronometer) dialogView.findViewById(R.id.chronometer));

        resultDialog.setAdapter(new SelectListAdapter<Action>(dialogActions,
                                                              false,
                                                              MapActivity.this));

        GridView breakGridView = (GridView) dialogView.findViewById(R.id.gvActions);
        breakGridView.setAdapter(resultDialog.getAdapter());

        dialogBuilder.setCancelable(false);
        resultDialog.setDialog(dialogBuilder.create());
        resultDialog.getDialog().getWindow().getAttributes().windowAnimations =
                R.style.DialogAnimation;

        resultDialog.setTvDialogName((AutoResizeTextView) dialogView.findViewById(R.id.tvExhibitDialogName));
        resultDialog.setCancelButton((Button) dialogView.findViewById(R.id.bExbibitDialogCancel));
        resultDialog.setFinishButton((Button) dialogView.findViewById(R.id.bExbibitDialogFinish));

        return resultDialog;
    }

    private void dialogButtonHandler(DialogState dialogToClose,
                                     Integer requestCode,
                                     boolean cancel,
                                     SelectListAdapter<Action> toTakeResultFrom) {
        Integer timeSpentInDialog = dialogToClose.dismissDialogWithTime();
        if (cancel) {
            dialogReturn(requestCode, true, null, timeSpentInDialog);
        } else {
            dialogReturn(requestCode,
                         false,
                         toTakeResultFrom.getSelectedOptions(),
                         timeSpentInDialog);
        }
        openedDialogs--;
    }

    private void dialogReturn(Integer requestCode,
                              boolean canceled,
                              List<Integer> resultIds,
                              Integer timeSpentInDialog) {
        if (!canceled) {
            Integer eventToSaveCode = null;
            if (requestCode != null) {
                ExhibitSpot es = (ExhibitSpot) mapState.hotSpotsForFloor.get(requestCode - 1);
                eventToSaveCode = es.getExhibitId();

                if (mapState.lastExhibitTextView != null) {
                    mapState.lastExhibitTextView.setBackground(getResources().getDrawable(R.drawable.exhibit_back));
                }
                mapState.lastExhibitTextView = es.getExhibitTextView();
                es.getExhibitTextView()
                  .setBackground(getResources().getDrawable(R.drawable.exhibit_last_clicked_back));

                mapState.exhibitsOverlay.invalidate();
            }

            Log.i(LOG_TAG, "Actions clicked: " + resultIds.toString());
            final RaportEvent
                    eventToSave =
                    new RaportEvent(eventToSaveCode,
                                    timeSpentInDialog / Consts.MILLIS_IN_SEC.intValue(),
                                    resultIds);

            new Thread(new Runnable() {
                @Override
                public void run() {
                    ExperimentData.getInstance().addEventToCurrentRaport(eventToSave);
                }
            }).run();
        }
    }

    public void pauseClick(View view) {
        Log.i(LOG_TAG, "Clicked break button");
        if (openedDialogs == 0) {
            openedDialogs++;
            showExhibitDialog(true, getString(R.string.break_action), null);
        }
    }

    public void endClick(View view) {
        Log.i(LOG_TAG, "Clicked end button");

        Intent postSurvey = new Intent(getApplicationContext(), SurveyActivity.class);
        postSurvey.putExtra("type", Survey.SurveyType.AFTER);
        startActivity(postSurvey);
        finish();
    }

    @Override
    public void onBackPressed() {
        // overriden to stop back button from working
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        MenuInflater inflater = getMenuInflater();
        inflater.inflate(R.menu.map_activity_menu, menu);
        return true;
    }

    @Override
    protected void onPostCreate(Bundle savedInstanceState) {
        super.onPostCreate(savedInstanceState);
        drawerToggle.syncState();
    }

    @Override
    public boolean onOptionsItemSelected(final MenuItem item) {
        if (drawerToggle.onOptionsItemSelected(item)) {
            return true;
        } else if (item.getItemId() == R.id.action_next_floor) {
            changeFloor();
        }
        return super.onOptionsItemSelected(item);
    }

    private void setViews() {
        Log.i(LOG_TAG, "Setting views");
        setContentView(R.layout.map_activity_layout);
        rlRootLayout = (RelativeLayout) findViewById(R.id.rlRootViewMapActivity);
        setTitle(getString(R.string.pietro) + " " + currentFloorNum.toString());
        setDrawer();
        Log.i(LOG_TAG, "Views set");
    }

    private void setDrawer() {
        ((Chronometer) findViewById(R.id.chronometer)).start();
    }

    private void setActionBar() {
        DrawerLayout drawerLayout = (DrawerLayout) findViewById(R.id.drawer_layout);
        drawerToggle =
                new ActionBarDrawerToggle(this,
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

    @Override
    protected void onDestroy() {
        super.onDestroy();
        Log.i(LOG_TAG, "onDestroy execution");
        if (tileView != null) {
            tileView.setBitmapProvider(null);
            tileView.destroy();
        }
        NetworkHandler.getInstance().stopBgDownload();

        Log.i(LOG_TAG, "deleting from ExhibitsData observers list");
        ExhibitsData.getInstance().deleteObserver(this);
    }

    private void changeFloor() {
        if (tileView != null) {
            tileView.setVisibility(View.INVISIBLE);
        }
        if (currentFloorNum.equals(Consts.FLOOR1)) {
            // Change to 1 floor
            Log.i(LOG_TAG, "switching floor to 2");
            currentFloorNum = Consts.FLOOR2;
        } else {
            // Change to 0 floor
            Log.i(LOG_TAG, "switching floor to 1");
            currentFloorNum = Consts.FLOOR1;
        }

        setTitle(getString(R.string.pietro) + " " + currentFloorNum.toString());
        if (MapData.getInstance().mapForFloorExists(currentFloorNum)) {
            new MapChangeFloor().execute();
        } else {
            layoutMapMissing.setVisibility(View.VISIBLE);
        }
    }

    // Layout setting:
    private void prepareTileView(final Integer floor,
                                 final List<ScaleData> scalesList,
                                 final List<Resolution> tileSizes) {

        final Semaphore localUISynchronization = new Semaphore(0, true);
        runOnUiThread(() -> {
            if (tileView != null) {
                tileView.setVisibility(View.INVISIBLE);
                clearAllExhibitsOnMap();
                rlRootLayout.removeView(tileView);
                tileView.destroy();
            }

            tileView = new TileView(MapActivity.this);

            for (int i = 0; i < scalesList.size(); i++) {
                Log.i(LOG_TAG, tileSizes.get(i).toString() + " " + Integer.toString(i));
                ScaleData scale = scalesList.get(i);
                Resolution res = tileSizes.get(i);
                tileView.addDetailLevel(scale.getScaleValue(),
                                        scale.getScaleCode(),
                                        res.getWidth(),
                                        res.getHeight());
            }

            tileView.setBitmapProvider(new MapBitmapProvider(currentFloorNum));

            tileView.setSize(mapState.currentMapSize.getWidth(),
                             mapState.currentMapSize.getHeight());
            tileView.defineBounds(0,
                                  0,
                                  mapState.originalMapSize.getWidth(),
                                  mapState.originalMapSize.getHeight());


            tileView.setTransitionsEnabled(false);
            tileView.setShouldRecycleBitmaps(false);
            tileView.setShouldScaleToFit(true);
            tileView.setScaleLimits(MINIMUM_SCALE, MAXIMUM_SCALE);
            tileView.setScale(MINIMUM_SCALE);

            mapState.exhibitsOverlay = new RelativeLayout(MapActivity.this);
            tileView.addScalingViewGroup(mapState.exhibitsOverlay);

            tileView.requestRender();

            localUISynchronization.release();
        });

        localUISynchronization.acquireUninterruptibly();
    }

    private void setLayout(final boolean isMapReady) {
        final Semaphore localUISynchronization = new Semaphore(0, true);

        final Boolean currentFloorExists = MapData.getInstance().mapForFloorExists(currentFloorNum);

        runOnUiThread(() -> {
            rlRootLayout.removeAllViews();

            if (isMapReady) {
                RelativeLayout.LayoutParams
                        lpTileView =
                        new RelativeLayout.LayoutParams(RelativeLayout.LayoutParams.MATCH_PARENT,
                                                        RelativeLayout.LayoutParams.MATCH_PARENT);
                rlRootLayout.addView(tileView, lpTileView);
                tileView.setVisibility(View.INVISIBLE);
            }

            layoutLoading = addLoadingLayout(rlRootLayout, rlRootLayout.getContext());
            layoutMapMissing = addMapMissingLayout(rlRootLayout, rlRootLayout.getContext());
            if (currentFloorExists) {
                layoutLoading.setVisibility(View.VISIBLE);
                layoutMapMissing.setVisibility(View.INVISIBLE);
            } else {
                layoutLoading.setVisibility(View.INVISIBLE);
                layoutMapMissing.setVisibility(View.VISIBLE);
            }

            localUISynchronization.release();
        });

        localUISynchronization.acquireUninterruptibly();
    }

    private LinearLayout addLoadingLayout(RelativeLayout parent, Context c) {
        TextView tvLoading = new TextView(c);
        tvLoading.setText(getResources().getString(R.string.loading_map));
        tvLoading.setTextSize(20);
        LinearLayout.LayoutParams
                tvLp =
                new LinearLayout.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT,
                                              ViewGroup.LayoutParams.WRAP_CONTENT);

        ProgressBar pb = new ProgressBar(c, null, android.R.attr.progressBarStyleLarge);
        LinearLayout.LayoutParams
                pbLp =
                new LinearLayout.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT,
                                              ViewGroup.LayoutParams.WRAP_CONTENT);

        LinearLayout ll = new LinearLayout(c);
        ll.setOrientation(LinearLayout.VERTICAL);
        ll.setGravity(Gravity.CENTER_HORIZONTAL);
        RelativeLayout.LayoutParams
                lp =
                new RelativeLayout.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT,
                                                ViewGroup.LayoutParams.WRAP_CONTENT);
        lp.addRule(RelativeLayout.CENTER_IN_PARENT);

        ll.addView(tvLoading, tvLp);
        ll.addView(pb, pbLp);

        parent.addView(ll, lp);

        return ll;
    }

    private LinearLayout addMapMissingLayout(RelativeLayout parent, Context c) {
        TextView tvLoadingFailed = new TextView(c);
        tvLoadingFailed.setText(getResources().getString(R.string.map_missing));
        tvLoadingFailed.setTextSize(20);
        LinearLayout.LayoutParams
                tvLp =
                new LinearLayout.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT,
                                              ViewGroup.LayoutParams.WRAP_CONTENT);

        LinearLayout ll = new LinearLayout(c);
        ll.setOrientation(LinearLayout.VERTICAL);
        ll.setGravity(Gravity.CENTER_HORIZONTAL);
        RelativeLayout.LayoutParams
                lp =
                new RelativeLayout.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT,
                                                ViewGroup.LayoutParams.WRAP_CONTENT);
        lp.addRule(RelativeLayout.CENTER_IN_PARENT);

        ll.addView(tvLoadingFailed, tvLp);

        parent.addView(ll, lp);

        return ll;
    }

    // Exhibits handling:

    private void clearAllExhibitsOnMap() {
        final Semaphore localUISynchronization = new Semaphore(0, true);

        Log.i(LOG_TAG, "clearing all exhibits on map");

        runOnUiThread(() -> {
            for (HotSpot hs : mapState.hotSpotsForFloor) {
                tileView.removeHotSpot(hs);
            }
            mapState.hotSpotsForFloor.clear();

            mapState.exhibitsOverlay.removeAllViews();

            localUISynchronization.release();
        });

        localUISynchronization.acquireUninterruptibly();
    }

    private void addAllExhibitsToMap(List<Exhibit> exhibits) {
        Log.i(LOG_TAG, "adding all exhibits to map");
        AutoResizeTextView artv;
        RelativeLayout.LayoutParams tvLP;
        ExhibitSpot es;
        Integer posX, posY, width, height;
        GradientDrawable bcgDrawable;

        HotSpot.HotSpotTapListener exhibitsListener = new ExhibitTapListener();

        final ArrayList<Pair<View, RelativeLayout.LayoutParams>> viewArrayList = new ArrayList<>();
        Integer floorNum = 1;
        for (Exhibit e : exhibits) {
            posX =
                    ImageHelper.getDimensionWhenScaleApplied(e.getX(),
                                                             mapState.originalMapSize.getWidth(),
                                                             mapState.currentMapSize.getWidth());
            posY =
                    ImageHelper.getDimensionWhenScaleApplied(e.getY(),
                                                             mapState.originalMapSize.getHeight(),
                                                             mapState.currentMapSize.getHeight());
            width =
                    ImageHelper.getDimensionWhenScaleApplied(e.getWidth(),
                                                             mapState.originalMapSize.getWidth(),
                                                             mapState.currentMapSize.getWidth());
            height =
                    ImageHelper.getDimensionWhenScaleApplied(e.getHeight(),
                                                             mapState.originalMapSize.getHeight(),
                                                             mapState.currentMapSize.getHeight());

            artv = new AutoResizeTextView(this);
            artv.setText(e.getName());
            artv.setTextColor(ColorHelper.getRgbHexTextColorForBackground(e.getColor()));

            //TODO - hardcoded background color and border color - to change later
            bcgDrawable = new GradientDrawable();
            bcgDrawable.setColor(e.getColor());
            bcgDrawable.setStroke(5, ColorHelper.getRgbBorderHexForColor(e.getColor()));
            artv.setBackgroundDrawable(bcgDrawable);

            artv.setGravity(Gravity.CENTER);
            artv.setMinTextSize(2f);
            artv.setTextSize(100f);
            artv.setSingleLine(false);

            tvLP = new RelativeLayout.LayoutParams(width, height);
            tvLP.setMargins(posX, posY, 0, 0);

            viewArrayList.add(new Pair<View, RelativeLayout.LayoutParams>(artv, tvLP));

            es = new ExhibitSpot(e.getId(), floorNum++, e.getName(), artv);
            es.setTag(this);
            es.set(new Rect(posX, posY, posX + width, posY + height));
            es.setHotSpotTapListener(exhibitsListener);
            mapState.hotSpotsForFloor.add(es);
        }

        final Semaphore waitOnUI = new Semaphore(0, true);

        runOnUiThread(() -> {
            for (int i = 0; i < viewArrayList.size(); i++) {
                mapState.exhibitsOverlay.addView(viewArrayList.get(i).first,
                                                 viewArrayList.get(i).second);
                tileView.addHotSpot(mapState.hotSpotsForFloor.get(i));
            }

            layoutLoading.setVisibility(View.INVISIBLE);
            tileView.setVisibility(View.VISIBLE);
            waitOnUI.release();
        });

        waitOnUI.acquireUninterruptibly();
    }

    // Action listeners:
    private class ExhibitTapListener implements HotSpot.HotSpotTapListener {
        @Override
        public void onHotSpotTap(final HotSpot hotSpot, int x, int y) {
            Log.i(LOG_TAG, "exhibit hotSpot clicked, x=" + Integer.toString(x) + " y=" +
                           Integer.toString(y));
            Log.i(LOG_TAG, "open dialogs: " + openedDialogs.toString());
            if (openedDialogs == 0) {
                openedDialogs++;
                // only if exhibit is clicked first time

                final Integer floorId = ((ExhibitSpot) hotSpot).getListId();
                showExhibitDialog(false, ((ExhibitSpot) hotSpot).getName(), floorId);
            }
        }
    }

    private void showExhibitDialog(boolean isBreak, String name, Integer requestCode) {
        if (isBreak) {
            startDialog(breakDialog, name, requestCode);
        } else {
            startDialog(exhibitDialog, name, requestCode);
        }
    }

    private void startDialog(final DialogState dialogState,
                             String name,
                             final Integer requestCode) {
        dialogState.getTvDialogName().setText(name);
        dialogState.getTvDialogName().setMinTextSize(2f);
        dialogState.getTvDialogName().setTextSize(100f);

        dialogState.getAdapter().unselectAllOptions();

        dialogState.getTimer().setBase(SystemClock.elapsedRealtime());
        dialogState.getTimer().start();

        dialogState.getCancelButton().setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                dialogButtonHandler(dialogState, requestCode, true, dialogState.getAdapter());
            }
        });
        dialogState.getFinishButton().setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                dialogButtonHandler(dialogState, requestCode, false, dialogState.getAdapter());
            }
        });

        dialogState.showDialog();
    }

    private void showAlert() {
        AlertDialog.Builder alert = new AlertDialog.Builder(this);
        alert.setTitle(R.string.error);
        alert.setMessage(R.string.mapIncompleteMessage);
        alert.setPositiveButton("OK", (dialog, which) -> {
            finish();
        });
        alert.setCancelable(false);
        alert.show();
    }
}
