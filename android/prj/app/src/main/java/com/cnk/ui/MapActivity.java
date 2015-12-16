package com.cnk.ui;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.graphics.Rect;
import android.graphics.drawable.Drawable;
import android.os.AsyncTask;
import android.os.Bundle;
import android.util.Log;
import android.util.Pair;
import android.view.Gravity;
import android.view.View;
import android.view.ViewGroup;
import android.widget.CompoundButton;
import android.widget.LinearLayout;
import android.widget.ProgressBar;
import android.widget.RelativeLayout;
import android.widget.Switch;
import android.widget.TextView;
import android.widget.Toast;

import com.cnk.R;
import com.cnk.communication.NetworkHandler;
import com.cnk.data.DataHandler;
import com.cnk.data.Resolution;
import com.cnk.database.DatabaseHelper;
import com.cnk.database.models.DetailLevelRes;
import com.cnk.database.models.Exhibit;
import com.qozix.tileview.TileView;
import com.qozix.tileview.hotspots.HotSpot;

import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;
import java.util.Observable;
import java.util.Observer;
import java.util.concurrent.Semaphore;

public class MapActivity extends Activity implements Observer {
    private static final String LOG_TAG = "MapActivity";

    private TileView tileView;
    private Semaphore changeAndUpdateMutex;
    private MapState mapState;
    private Switch floorsSwitch;
    private LinearLayout layoutLoading, layoutMapMissing;
    private Integer currentFloorNum;
    private NetworkHandler networkHandler;
    private Boolean fullyLoaded;
    RelativeLayout rlRootLayout;

    private static final Float MAXIMUM_SCALE = 4.0f;
    private static final Float MINIMUM_SCALE = 0.01f;

    public static final Integer TILE_SIDE_LEN = 256;

    // Android activity lifecycle overriden methods:

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        Log.i(LOG_TAG, "onCreate execution");

        currentFloorNum = 0;

        rlRootLayout = new RelativeLayout(this);
        setContentView(rlRootLayout);

        changeAndUpdateMutex = new Semaphore(1, true);

        mapState = new MapState();
        mapState.hotSpotsForFloor = new ArrayList<>();
        mapState.exhibitsOverlay = new RelativeLayout(this);
        mapState.floorReady = new Boolean[2];
        mapState.floorReady[0] = false;
        mapState.floorReady[1] = false;

        new StartUpTask().execute();

        networkHandler = new NetworkHandler();
        networkHandler.startBgDownload();
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
    protected void onStop() {
        super.onStop();

        if (networkHandler != null) {
            networkHandler.stopBgDownload();
        }
    }

    @Override
    protected void onPause() {
        super.onPause();

        Log.i(LOG_TAG, "onPause execution");
        if (tileView != null) {
            tileView.pause();
        }

        Log.i(LOG_TAG, "deleting from DataHandler observers list");
        DataHandler.getInstance().deleteObserver(this);
    }

    @Override
    protected void onResume() {
        super.onResume();

        Log.i(LOG_TAG, "onResume execution");
        if (tileView != null) {
            tileView.resume();
        }

        if (networkHandler != null) {
            networkHandler.startBgDownload();
        }

        Log.i(LOG_TAG, "adding to DataHandler observers list");
        DataHandler.getInstance().addObserver(this);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();

        Log.i(LOG_TAG, "onDestroy execution");
        tileView.setBitmapProvider(null);
        tileView.destroy();
        networkHandler.stopBgDownload();
    }


    // Layout setting:

    private void prepareTileView(final List<ScaleData> scalesList) {

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

                for (ScaleData scale : scalesList) {
                    tileView.addDetailLevel(scale.getScaleValue(), scale.getScaleCode(),
                            TILE_SIDE_LEN, TILE_SIDE_LEN);
                }

                tileView.setBitmapProvider(new MapBitmapProvider(currentFloorNum));

                tileView.setSize(mapState.currentMapSize.getWidth(), mapState.currentMapSize.getHeight());
                tileView.defineBounds(0, 0, mapState.originalMapSize.getWidth(), mapState.originalMapSize.getHeight());

                tileView.setTransitionsEnabled(false);
                tileView.setShouldRecycleBitmaps(false);
                tileView.setShouldScaleToFit(true);
                tileView.setScaleLimits(MINIMUM_SCALE, MAXIMUM_SCALE);
                tileView.setScale(0.01f);

                mapState.exhibitsOverlay = new RelativeLayout(MapActivity.this);
                tileView.addScalingViewGroup(mapState.exhibitsOverlay);

                tileView.requestRender();

                localUISynchronization.release();
            }
        });

        localUISynchronization.acquireUninterruptibly();
    }

    private void setLayout(final Boolean isMapReady) {
        final Semaphore localUISynchronization = new Semaphore(0, true);

        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                rlRootLayout.removeAllViews();

                if (isMapReady != null && isMapReady) {
                    RelativeLayout.LayoutParams lpTileView = new RelativeLayout.LayoutParams(RelativeLayout.LayoutParams.MATCH_PARENT,
                            RelativeLayout.LayoutParams.MATCH_PARENT);
                    rlRootLayout.addView(tileView, lpTileView);
                    tileView.setVisibility(View.INVISIBLE);
                }

                layoutLoading = addLoadingLayout(rlRootLayout, rlRootLayout.getContext());
                layoutMapMissing = addLoadingLayout(rlRootLayout, rlRootLayout.getContext());
                floorsSwitch = addFloorsSwitch(rlRootLayout, rlRootLayout.getContext());
                floorsSwitch.setVisibility(View.INVISIBLE);

                if (DataHandler.getInstance().mapForFloorExists(currentFloorNum)) {
                    layoutLoading.setVisibility(View.VISIBLE);
                    layoutMapMissing.setVisibility(View.INVISIBLE);
                } else {
                    layoutLoading.setVisibility(View.INVISIBLE);
                    layoutLoading.setVisibility(View.VISIBLE);
                }

                localUISynchronization.release();
            }
        });

        localUISynchronization.acquireUninterruptibly();
    }

    private Switch addFloorsSwitch(RelativeLayout parent, Context c) {
        Switch s = new Switch(c);
        s.setTextOff(getResources().getString(R.string.pietro1));
        s.setTextOn(getResources().getString(R.string.pietro2));
        s.setOnCheckedChangeListener(new FloorsSwitchListener());

        RelativeLayout.LayoutParams lp = new RelativeLayout.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT, ViewGroup.LayoutParams.WRAP_CONTENT);
        lp.addRule(RelativeLayout.ALIGN_PARENT_TOP);
        lp.addRule(RelativeLayout.ALIGN_PARENT_LEFT);
        lp.setMargins(20, 20, 20, 20);

        parent.addView(s, lp);
        return s;
    }

    private LinearLayout addLoadingLayout(RelativeLayout parent, Context c) {
        TextView tvLoading = new TextView(c);
        tvLoading.setText(getResources().getString(R.string.loading_map));
        tvLoading.setTextSize(20);
        LinearLayout.LayoutParams tvLp = new LinearLayout.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT, ViewGroup.LayoutParams.WRAP_CONTENT);

        ProgressBar pb = new ProgressBar(c);
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
        tvLoadingFailed.setText(getResources().getString(R.string.map_wait));
        tvLoadingFailed.setTextSize(20);
        LinearLayout.LayoutParams tvLp = new LinearLayout.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT, ViewGroup.LayoutParams.WRAP_CONTENT);

        ProgressBar pb = new ProgressBar(c);
        LinearLayout.LayoutParams pbLp = new LinearLayout.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT, ViewGroup.LayoutParams.WRAP_CONTENT);

        LinearLayout ll = new LinearLayout(c);
        ll.setOrientation(LinearLayout.VERTICAL);
        ll.setGravity(Gravity.CENTER_HORIZONTAL);
        RelativeLayout.LayoutParams lp = new RelativeLayout.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT, ViewGroup.LayoutParams.WRAP_CONTENT);
        lp.addRule(RelativeLayout.CENTER_IN_PARENT);

        ll.addView(tvLoadingFailed, tvLp);
        ll.addView(pb, pbLp);

        parent.addView(ll, lp);

        return ll;
    }


    // Data updating:

    @Override
    public void update(Observable observable, Object o) {
        DataHandler.Item notification = (DataHandler.Item) o;

        if (notification.equals(DataHandler.Item.BOTH_FLOORS_MAP_CHANGING)) {
            mapState.floorReady[0] = false;
            mapState.floorReady[1] = false;
            Log.i(LOG_TAG, "Notification - both floors map is being changed");
            runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    MapActivity.this.tileView.setVisibility(View.INVISIBLE);
                    MapActivity.this.layoutLoading.setVisibility(View.VISIBLE);
                }
            });
        } else if (notification.equals(DataHandler.Item.BOTH_FLOORS_MAP_CHANGED)) {
            mapState.floorReady[0] = true;
            mapState.floorReady[1] = true;
            Log.i(LOG_TAG, "Notification - both floors map has been changed");
            new MapChangeFloor().execute();
        } else if (notification.equals(DataHandler.Item.FLOOR_0_MAP_CHANGING)) {
            mapState.floorReady[0] = false;
            Log.i(LOG_TAG, "Notification - floor 0 map is being changed");
            if (currentFloorNum == 0) {
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        MapActivity.this.tileView.setVisibility(View.INVISIBLE);
                        MapActivity.this.layoutLoading.setVisibility(View.VISIBLE);
                    }
                });
            }
        } else if (notification.equals(DataHandler.Item.FLOOR_0_MAP_CHANGED)) {
            mapState.floorReady[0] = true;
            Log.i(LOG_TAG, "Notification - floor 0 map has been changed");
            if (currentFloorNum == 0) {
                new MapChangeFloor().execute();
            }
        } else if (notification.equals(DataHandler.Item.FLOOR_1_MAP_CHANGING)) {
            mapState.floorReady[1] = false;
            Log.i(LOG_TAG, "Notification - floor 1 map is being changed");
            if (currentFloorNum == 1) {
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        MapActivity.this.tileView.setVisibility(View.INVISIBLE);
                        MapActivity.this.layoutLoading.setVisibility(View.VISIBLE);
                    }
                });
            }
        } else if (notification.equals(DataHandler.Item.FLOOR_1_MAP_CHANGED)) {
            mapState.floorReady[1] = true;
            Log.i(LOG_TAG, "Notification - floor 1 map has been changed");
            if (currentFloorNum == 1) {
                new MapChangeFloor().execute();
            }
        }
        else if (notification.equals(DataHandler.Item.EXHIBITS)) {
            Log.i(LOG_TAG, "Received exhibits update notification");
            new MapRefreshExhibits().execute();
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
            addAllExhibitsToMap(getExhibitsForFloor(currentFloorNum));

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
            floor = currentFloorNum;

            assert (floor == 0 || floor == 1);

            System.gc();

            final Semaphore localUISynchronization = new Semaphore(0, true);

            changeAndUpdateMutex.acquireUninterruptibly();
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

            clearAllExhibitsOnMap();

            prepareTileView(ll);
            setLayout(true);

            addAllExhibitsToMap(getExhibitsForFloor(floor));

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

    private List<Exhibit> getExhibitsForFloor(Integer floor) {
        return DataHandler.getInstance().getExhibitsOfFloor(floor);
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
            artv.setTextSize(100f);
            artv.setMaxLines(10);

            tvLP = new RelativeLayout.LayoutParams(width, height);
            tvLP.setMargins(posX, posY, 0, 0);

            viewArrayList.add(new Pair<View, RelativeLayout.LayoutParams>(artv, tvLP));

            es = new ExhibitSpot(e.getId());
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
                floorsSwitch.setVisibility(View.VISIBLE);

                waitOnUI.release();
            }
        });

        waitOnUI.acquireUninterruptibly();
    }


    // Private classes declarations:

    private class ExhibitSpot extends HotSpot {
        private Integer exhibitId;

        public ExhibitSpot(Integer exhibitId) {
            super();
            this.exhibitId = exhibitId;
        }

        public Integer getExhibitId() {
            return exhibitId;
        }
    }

    private class MapState {
        Boolean floorReady[];
        Resolution currentMapSize, originalMapSize;

        List<HotSpot> hotSpotsForFloor;
        RelativeLayout exhibitsOverlay;
    }


    // Action listeners:

    private class ExhibitTapListener implements HotSpot.HotSpotTapListener {
        @Override
        public void onHotSpotTap(HotSpot hotSpot, int x, int y) {
            Log.i(LOG_TAG, "exhibit hotSpot clicked, x=" + Integer.toString(x) + " y=" + Integer.toString(y));
            Integer id = ((ExhibitSpot) hotSpot).getExhibitId();
            Toast.makeText(getApplicationContext(), "Clicked exhibit with id: " + id.toString(), Toast.LENGTH_SHORT).show();


            /*Intent exhibitWindowIntent = new Intent(MapActivity.this, ExhibitDialog.class);
            exhibitWindowIntent.putStringArrayListExtra("Available actions", new ArrayList<String>());
            startActivityForResult(exhibitWindowIntent, id);*/
        }
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);

        Toast.makeText(getApplicationContext(), "Back from exhibit with id: " + Integer.toString(requestCode) +
                " result code: " + Integer.toString(resultCode), Toast.LENGTH_LONG).show();

    }

    private class FloorsSwitchListener implements CompoundButton.OnCheckedChangeListener {
        @Override
        public void onCheckedChanged(CompoundButton compoundButton, boolean b) {
            //floorsSwitch.setEnabled(false);
            floorsSwitch.setVisibility(View.INVISIBLE);
            tileView.setVisibility(View.INVISIBLE);
            if (b) {
                // Change to 1 floor
                Log.i(LOG_TAG, "switching floor to 1");
                currentFloorNum = DatabaseHelper.floor1Code;
            } else {
                // Change to 0 floor
                Log.i(LOG_TAG, "switching floor to 0");
                currentFloorNum = DatabaseHelper.floor0Code;
            }

            new MapChangeFloor().execute();
        }
    }
}
