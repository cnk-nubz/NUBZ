package com.cnk.ui;

import android.app.ActivityOptions;
import android.app.ProgressDialog;
import android.content.Context;
import android.content.Intent;
import android.graphics.Point;
import android.graphics.Rect;
import android.graphics.drawable.Drawable;
import android.os.AsyncTask;
import android.os.Bundle;
import android.support.v4.widget.DrawerLayout;
import android.support.v7.app.ActionBarDrawerToggle;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.util.Pair;
import android.view.Gravity;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Chronometer;
import android.widget.LinearLayout;
import android.widget.ProgressBar;
import android.widget.RelativeLayout;
import android.widget.TextView;

import com.cnk.R;
import com.cnk.StartScreen;
import com.cnk.communication.NetworkHandler;
import com.cnk.data.DataHandler;
import com.cnk.data.Resolution;
import com.cnk.database.models.DetailLevelRes;
import com.cnk.database.models.Exhibit;
import com.cnk.ui.exhibitwindow.ExhibitDialog;
import com.cnk.utilities.Consts;
import com.cnk.utilities.Util;
import com.qozix.tileview.TileView;
import com.qozix.tileview.hotspots.HotSpot;

import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;
import java.util.Observable;
import java.util.Observer;
import java.util.concurrent.Semaphore;

public class MapActivity extends AppCompatActivity implements Observer {
    private static final String LOG_TAG = "MapActivity";
    private static final String BREAK_NAME = "Przerwa";
    private static final String TITLE_PREFIX = "Piętro";
    private static final Integer BREAK_ID = 0;
    private static final Float MAXIMUM_SCALE = 4.0f;
    private static final Float MINIMUM_SCALE = 0.01f;

    private TileView tileView;
    private Semaphore changeAndUpdateMutex;
    private MapState mapState;
    private LinearLayout layoutLoading, layoutMapMissing;
    private Integer currentFloorNum;
    private NetworkHandler networkHandler;
    private RelativeLayout rlRootLayout;
    private RelativeLayout voidLayout;
    private Point lastClick;
    private ProgressDialog spinner;
    private ActionBarDrawerToggle drawerToggle;
    private Integer openedDialogs;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        Log.i(LOG_TAG, "onCreate execution");
        currentFloorNum = 0;
        changeAndUpdateMutex = new Semaphore(1, true);
        openedDialogs = 0;
        networkHandler = new NetworkHandler();

        Log.i(LOG_TAG, "adding to DataHandler observers list");
        DataHandler.getInstance().addObserver(this);

        setViews();
        setActionBar();
        setSpinner();

        networkHandler.downloadExperimentData();
    }

    public void pauseClick(View view) {
        Log.i(LOG_TAG, "Clicked break button");
        showExhibitDialog(true, BREAK_NAME, BREAK_ID);
    }

    public void endClick(View view) {
        Log.i(LOG_TAG, "Clicked end button");
        Intent startScreenIntent = new Intent(getApplicationContext(), StartScreen.class);
        startActivity(startScreenIntent);
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
        setTitle(TITLE_PREFIX + " " + currentFloorNum.toString());
        setDrawer();
        Log.i(LOG_TAG, "Views set");
    }

    private void setDrawer() {
        ((Chronometer) findViewById(R.id.chronometer)).start();
    }

    private void setActionBar() {
        DrawerLayout drawerLayout = (DrawerLayout) findViewById(R.id.drawer_layout);
        drawerToggle = new ActionBarDrawerToggle(
                this,
                drawerLayout,
                R.string.drawer_open,
                R.string.drawer_close
        ) {

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

    private void setSpinner() {
        spinner = new ProgressDialog(this);
        spinner.setTitle("Ładowanie");
        spinner.setMessage("Oczekiwanie na pobranie akcji");
        spinner.show();
    }

    private class StartUpTask extends AsyncTask<Void, Void, Boolean> {
        @Override
        protected Boolean doInBackground(Void... voids) {
            if (DataHandler.getInstance().mapForFloorExists(currentFloorNum)) {
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

    @Override
    public boolean dispatchTouchEvent(MotionEvent ev) {
        lastClick = new Point((int) ev.getX(), (int) ev.getY());
        return super.dispatchTouchEvent(ev);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();

        Log.i(LOG_TAG, "onDestroy execution");
        if (tileView != null) {
            tileView.setBitmapProvider(null);
            tileView.destroy();
        }

        networkHandler.stopBgDownload();

        Log.i(LOG_TAG, "deleting from DataHandler observers list");
        DataHandler.getInstance().deleteObserver(this);
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
        setTitle(TITLE_PREFIX + " " + currentFloorNum.toString());
        if (DataHandler.getInstance().mapForFloorExists(currentFloorNum)) {
            new MapChangeFloor().execute();
        } else {
            layoutMapMissing.setVisibility(View.VISIBLE);
        }
    }


    // Layout setting:
    private void prepareTileView(final List<ScaleData> scalesList, final List<Resolution> tileSizes) {

        final Semaphore localUISynchronization = new Semaphore(0, true);
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
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
                    tileView.addDetailLevel(scale.getScaleValue(), scale.getScaleCode(),
                            res.getWidth(), res.getHeight());
                }

                tileView.setBitmapProvider(new MapBitmapProvider(currentFloorNum));

                tileView.setSize(mapState.currentMapSize.getWidth(), mapState.currentMapSize.getHeight());
                tileView.defineBounds(0, 0, mapState.originalMapSize.getWidth(), mapState.originalMapSize.getHeight());

                tileView.setTransitionsEnabled(false);
                tileView.setShouldRecycleBitmaps(false);
                tileView.setShouldScaleToFit(true);
                tileView.setScaleLimits(MINIMUM_SCALE, MAXIMUM_SCALE);
                tileView.setScale(MINIMUM_SCALE);

                mapState.exhibitsOverlay = new RelativeLayout(MapActivity.this);
                tileView.addScalingViewGroup(mapState.exhibitsOverlay);

                tileView.requestRender();

                localUISynchronization.release();
            }
        });

        localUISynchronization.acquireUninterruptibly();
    }

    private void setLayout(final boolean isMapReady) {
        final Semaphore localUISynchronization = new Semaphore(0, true);

        final Boolean currentFloorExists = DataHandler.getInstance().mapForFloorExists(currentFloorNum);

        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                rlRootLayout.removeAllViews();

                if (isMapReady) {
                    RelativeLayout.LayoutParams lpTileView = new RelativeLayout.LayoutParams(RelativeLayout.LayoutParams.MATCH_PARENT,
                            RelativeLayout.LayoutParams.MATCH_PARENT);
                    rlRootLayout.addView(tileView, lpTileView);
                    tileView.setVisibility(View.INVISIBLE);
                }

                voidLayout = addVoidLayout(rlRootLayout, rlRootLayout.getContext());
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
            }
        });

        localUISynchronization.acquireUninterruptibly();
    }

    private RelativeLayout addVoidLayout(RelativeLayout parent, Context c) {
        RelativeLayout rl = new RelativeLayout(c);
        RelativeLayout.LayoutParams lp = new RelativeLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT);

        parent.addView(rl, lp);

        return rl;
    }

    private LinearLayout addLoadingLayout(RelativeLayout parent, Context c) {
        TextView tvLoading = new TextView(c);
        tvLoading.setText(getResources().getString(R.string.loading_map));
        tvLoading.setTextSize(20);
        LinearLayout.LayoutParams tvLp = new LinearLayout.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT, ViewGroup.LayoutParams.WRAP_CONTENT);

        ProgressBar pb = new ProgressBar(c, null, android.R.attr.progressBarStyleLarge);
        LinearLayout.LayoutParams pbLp = new LinearLayout.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT, ViewGroup.LayoutParams.WRAP_CONTENT);

        LinearLayout ll = new LinearLayout(c);
        ll.setOrientation(LinearLayout.VERTICAL);
        ll.setGravity(Gravity.CENTER_HORIZONTAL);
        RelativeLayout.LayoutParams lp = new RelativeLayout.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT, ViewGroup.LayoutParams.WRAP_CONTENT);
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
        LinearLayout.LayoutParams tvLp = new LinearLayout.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT, ViewGroup.LayoutParams.WRAP_CONTENT);

        LinearLayout ll = new LinearLayout(c);
        ll.setOrientation(LinearLayout.VERTICAL);
        ll.setGravity(Gravity.CENTER_HORIZONTAL);
        RelativeLayout.LayoutParams lp = new RelativeLayout.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT, ViewGroup.LayoutParams.WRAP_CONTENT);
        lp.addRule(RelativeLayout.CENTER_IN_PARENT);

        ll.addView(tvLoadingFailed, tvLp);

        parent.addView(ll, lp);

        return ll;
    }


    // Data updating:
    @Override
    public void update(Observable observable, Object o) {
        DataHandler.Item notification = (DataHandler.Item) o;
        if (notification.equals(DataHandler.Item.EXHIBITS)) {
            Log.i(LOG_TAG, "Received exhibits update notification");
            new MapRefreshExhibits().execute();
        } else if (notification.equals(DataHandler.Item.EXPERIMENT_DATA)) {
            experimentDataDownloaded();
        }
    }

    private void experimentDataDownloaded() {
        Util.waitDelay(Consts.SECOND);
        spinner.dismiss();
        mapState = new MapState(this);
        new StartUpTask().execute();
        Log.i(LOG_TAG, "starting background download");
        networkHandler.startBgDownload();
        spinner.dismiss();
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
            addAllExhibitsToMap(DataHandler.getInstance().getExhibitsOfFloor(currentFloorNum));

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

            Integer detailLevels = DataHandler.getInstance().getDetailLevelsCountForFloor(floor);
            DetailLevelRes biggestResolution = DataHandler.getInstance().getDetailLevelResolution(floor, detailLevels - 1);

            mapState.currentMapSize = biggestResolution.getScaledRes();
            mapState.originalMapSize = DataHandler.getInstance().getOriginalResolution(floor);

            LinkedList<ScaleData> ll = new LinkedList<>();
            for (int i = 0; i < detailLevels; i++) {
                DetailLevelRes current = DataHandler.getInstance().getDetailLevelResolution(floor, i);
                ll.add(new ScaleData((float) current.getScaledRes().getWidth() / biggestResolution.getScaledRes().getWidth(), i));
            }

            ArrayList<Resolution> tileSizes = new ArrayList<>();
            for (int i = 0; i < detailLevels; i++) {
                Resolution current = DataHandler.getInstance().getTileSize(floor, i);
                tileSizes.add(current);
            }

            prepareTileView(ll, tileSizes);
            setLayout(true);

            addAllExhibitsToMap(DataHandler.getInstance().getExhibitsOfFloor(floor));

            changeAndUpdateMutex.release();

            Log.i(LOG_TAG, "map setting end");
            return null;
        }
    }

    // Exhibits handling:

    private void clearAllExhibitsOnMap() {
        final Semaphore localUISynchronization = new Semaphore(0, true);

        Log.i(LOG_TAG, "clearing all exhibits on map");

        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                for (HotSpot hs : mapState.hotSpotsForFloor) {
                    tileView.removeHotSpot(hs);
                }
                mapState.hotSpotsForFloor.clear();

                mapState.exhibitsOverlay.removeAllViews();

                localUISynchronization.release();
            }
        });

        localUISynchronization.acquireUninterruptibly();
    }

    private void addAllExhibitsToMap(List<Exhibit> exhibits) {
        Log.i(LOG_TAG, "adding all exhibits to map");
        AutoResizeTextView artv;
        RelativeLayout.LayoutParams tvLP;
        ExhibitSpot es;
        Integer posX, posY, width, height;
        Drawable bcgDrawable;

        HotSpot.HotSpotTapListener exhibitsListener = new ExhibitTapListener();

        final ArrayList<Pair<View, RelativeLayout.LayoutParams>> viewArrayList = new ArrayList<>();
        Integer floorNum = 1;
        for (Exhibit e: exhibits) {
            posX = ImageHelper.getDimensionWhenScaleApplied(e.getX(),
                    mapState.originalMapSize.getWidth(),
                    mapState.currentMapSize.getWidth());
            posY = ImageHelper.getDimensionWhenScaleApplied(e.getY(),
                    mapState.originalMapSize.getHeight(),
                    mapState.currentMapSize.getHeight());
            width = ImageHelper.getDimensionWhenScaleApplied(e.getWidth(),
                    mapState.originalMapSize.getWidth(),
                    mapState.currentMapSize.getWidth());
            height = ImageHelper.getDimensionWhenScaleApplied(e.getHeight(),
                    mapState.originalMapSize.getHeight(),
                    mapState.currentMapSize.getHeight());

            artv = new AutoResizeTextView(this);
            artv.setText(e.getName());

            //TODO - hardcoded background color and border color - to change later
            bcgDrawable = getResources().getDrawable(R.drawable.exhibit_back);
            artv.setBackground(bcgDrawable);

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

        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                for (int i = 0; i < viewArrayList.size(); i++) {
                    mapState.exhibitsOverlay.addView(viewArrayList.get(i).first, viewArrayList.get(i).second);
                    tileView.addHotSpot(mapState.hotSpotsForFloor.get(i));
                }

                layoutLoading.setVisibility(View.INVISIBLE);
                tileView.setVisibility(View.VISIBLE);
                waitOnUI.release();
            }
        });

        waitOnUI.acquireUninterruptibly();
    }


    // Private classes declarations:
    private class ExhibitSpot extends HotSpot {
        private Integer exhibitId;
        private Integer listId;
        private String name;
        private AutoResizeTextView exhibitTextView;

        public ExhibitSpot(Integer exhibitId, Integer listId, String name, AutoResizeTextView exhibitTextView) {
            super();
            this.listId = listId;
            this.exhibitId = exhibitId;
            this.name = name;
            this.exhibitTextView = exhibitTextView;
        }

        public Integer getExhibitId() {
            return exhibitId;
        }

        public String getName() {
            return name;
        }

        public AutoResizeTextView getExhibitTextView() {
            return exhibitTextView;
        }

        public Integer getListId() {
            return listId;
        }
    }

    private class MapState {
        public MapState(MapActivity activity) {
            hotSpotsForFloor = new ArrayList<>();
            exhibitsOverlay = new RelativeLayout(activity);
        }

        Resolution currentMapSize, originalMapSize;

        ArrayList<HotSpot> hotSpotsForFloor;
        RelativeLayout exhibitsOverlay;

        AutoResizeTextView lastExhibitTextView;
    }


    // Action listeners:
    private class ExhibitTapListener implements HotSpot.HotSpotTapListener {
        @Override
        public void onHotSpotTap(final HotSpot hotSpot, int x, int y) {
            Log.i(LOG_TAG, "exhibit hotSpot clicked, x=" + Integer.toString(x) + " y=" + Integer.toString(y));
            if (openedDialogs == 0) {
                openedDialogs++;
                // only if exhibit is clicked first time

                final Integer floorId = ((ExhibitSpot) hotSpot).listId;
                showExhibitDialog(false, ((ExhibitSpot) hotSpot).getName(), floorId);
            }
        }
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);

        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                openedDialogs--;
            }
        });

        if (resultCode == RESULT_OK) {
            if (requestCode != BREAK_ID) {
                ExhibitSpot es = (ExhibitSpot) mapState.hotSpotsForFloor.get(requestCode - 1);
                if (mapState.lastExhibitTextView != null) {
                    mapState.lastExhibitTextView
                            .setBackground(getResources().getDrawable(R.drawable.exhibit_back));
                }
                mapState.lastExhibitTextView = es.getExhibitTextView();
                es.getExhibitTextView()
                        .setBackground(getResources().getDrawable(R.drawable.exhibit_last_clicked_back));

                mapState.exhibitsOverlay.invalidate();

                ArrayList<String> selectedActions = data.getStringArrayListExtra(ExhibitDialog.SELECTED_ACTIONS);
            } else {
                // TODO: after break
            }
        }
    }

    private void showExhibitDialog(boolean isBreak, String name, Integer requestCode) {
        Intent exhibitWindowIntent = new Intent(MapActivity.this, ExhibitDialog.class);
        exhibitWindowIntent.putExtra(ExhibitDialog.NAME, name);
        exhibitWindowIntent.putExtra(ExhibitDialog.IS_BREAK, isBreak);
        ActivityOptions activityOptions =
            ActivityOptions.makeScaleUpAnimation(voidLayout, lastClick.x, lastClick.y, 1, 1);
        startActivityForResult(exhibitWindowIntent, requestCode, activityOptions.toBundle());
    }
}
