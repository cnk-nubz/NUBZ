package com.cnk.ui;

import android.app.Activity;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Rect;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Handler;
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
import com.cnk.database.DatabaseHelper;
import com.cnk.database.Exhibit;
import com.qozix.tileview.TileView;
import com.qozix.tileview.detail.DetailLevel;
import com.qozix.tileview.graphics.BitmapProvider;
import com.qozix.tileview.hotspots.HotSpot;

import java.util.ArrayList;
import java.util.List;
import java.util.Observable;
import java.util.Observer;
import java.util.concurrent.Semaphore;

public class MapActivity extends Activity implements Observer {

    private TileView tileView;
    private Semaphore changeAndUpdateMutex;
    private MapState mapState;
    private Switch floorsSwitch;
    private LinearLayout layoutLoading, layoutMapMissing;
    private Integer currentFloorNum;
    private NetworkHandler networkHandler;
    private Boolean fullyLoaded;


    // Android activity lifecycle overriden methods:

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        RelativeLayout rlRootLayout = new RelativeLayout(this);
        setContentView(rlRootLayout);

        currentFloorNum = 0;
        fullyLoaded = false;

        tileView = new TileView(this);

        RelativeLayout.LayoutParams lpTileView = new RelativeLayout.LayoutParams(RelativeLayout.LayoutParams.MATCH_PARENT, RelativeLayout.LayoutParams.MATCH_PARENT);
        rlRootLayout.addView(tileView, lpTileView);

        floorsSwitch = addFloorsSwitch(rlRootLayout, rlRootLayout.getContext());

        tileView.setVisibility(View.INVISIBLE);
        tileView.setTransitionsEnabled(false);
        tileView.setShouldRecycleBitmaps(false);
        tileView.setScaleLimits(0.01f, 4f);
        tileView.setScale(0.5f);

        layoutLoading = addLoadingLayout(rlRootLayout, rlRootLayout.getContext());
        layoutLoading.setVisibility(View.VISIBLE);

        layoutMapMissing = addMapMissingLayout(rlRootLayout, rlRootLayout.getContext());
        layoutMapMissing.setVisibility(View.INVISIBLE);

        changeAndUpdateMutex = new Semaphore(1, true);

        mapState = new MapState();
        mapState.hotSpotsForFloor = new ArrayList<>();
        mapState.exhibitsOverlay = new RelativeLayout(this);

        tileView.addScalingViewGroup(mapState.exhibitsOverlay);

        networkHandler = new NetworkHandler();
        networkHandler.startBgDownload();

        new MapChangeFloor().execute(currentFloorNum);
    }

    @Override
    protected void onPause() {
        super.onPause();
        tileView.pause();

        DataHandler.getInstance().deleteObserver(this);
    }

    @Override
    protected void onResume() {
        super.onResume();

        if (fullyLoaded) {
            tileView.resume();
            tileView.onDetailLevelChanged(
                    new DetailLevel(tileView.getDetailLevelManager(), 1.000f, 1, 256, 256));
        }
        fullyLoaded = true;

        DataHandler.getInstance().addObserver(this);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        tileView.setBitmapProvider(null);
        tileView.destroy();
        networkHandler.stopBgDownload();
    }


    // Layout adding:

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
        DataHandler.Item itemChanged = (DataHandler.Item) o;

        if (itemChanged.equals(DataHandler.Item.MAP)) {
            new MapChangeFloor().execute(currentFloorNum);
        } else if (itemChanged.equals(DataHandler.Item.EXHIBITS)) {
            new MapRefreshExhibits().execute();
        }
    }

    // Map changing:

    private class MapRefreshExhibits extends AsyncTask<Void, Void, Void> {
        @Override
        protected Void doInBackground(Void... voids) {
            changeAndUpdateMutex.acquireUninterruptibly();

            final Semaphore waitForUIMutex = new Semaphore(0, true);

            runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    tileView.setVisibility(View.INVISIBLE);
                    layoutLoading.setVisibility(View.VISIBLE);
                    clearAllExhibitsOnMap();
                    waitForUIMutex.release();
                }
            });

            waitForUIMutex.acquireUninterruptibly();
            getExhibitsForCurrentFloor();
            addAllExhibitsToMap();

            runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    layoutLoading.setVisibility(View.INVISIBLE);
                    tileView.setVisibility(View.VISIBLE);
                }
            });

            changeAndUpdateMutex.release();

            return null;
        }
    }

    private class MapChangeFloor extends AsyncTask<Integer, Void, Void> {
        private Integer floor;

        @Override
        protected Void doInBackground(Integer... integers) {
            floor = integers[0];
            assert (floor == 0 || floor == 1);

            System.gc();

            changeAndUpdateMutex.acquireUninterruptibly();

            Drawable mapDrawable = DataHandler.getInstance().getFloorMap(floor);
            if (mapDrawable == null) {
                changeAndUpdateMutex.release();
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        layoutLoading.setVisibility(View.INVISIBLE);
                        layoutMapMissing.setVisibility(View.VISIBLE);
                    }
                });
                cancel(true);
            } else {
                mapState.mapWidth = mapDrawable.getIntrinsicWidth();
                mapState.mapHeight = mapDrawable.getIntrinsicHeight();
            }

            final Semaphore localSynchronizationMutex = new Semaphore(0, true);
            
            runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    layoutMapMissing.setVisibility(View.INVISIBLE);
                    tileView.setVisibility(View.INVISIBLE);
                    layoutLoading.setVisibility(View.VISIBLE);

                    tileView.cancelRender();
                    tileView.setBitmapProvider(null);
                    localSynchronizationMutex.release();
                }
            });

            localSynchronizationMutex.acquireUninterruptibly();

            final BitmapProvider provider = prepareBitmapProvider(mapDrawable);
            // from here on mapDrawable is recycled and should not be used!

            runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    tileView.onDetailLevelChanged(
                            new DetailLevel(tileView.getDetailLevelManager(), 1.000f, 1, 256, 256));
                    tileView.setBitmapProvider(provider);
                    mapState.currentMapSize = ScaleHelper.getInterpolatedDimensions(mapState.mapWidth,
                            mapState.mapHeight);
                    tileView.setSize(mapState.currentMapSize.first, mapState.currentMapSize.second);
                    tileView.defineBounds(0, 0, mapState.currentMapSize.first, mapState.currentMapSize.second);

                    //start rendering again
                    tileView.requestRender();

                    tileView.setShouldScaleToFit(true);
                    tileView.setScale(0.02f);
                    System.gc();

                    localSynchronizationMutex.release();
                }
            });

            localSynchronizationMutex.acquireUninterruptibly();

            runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    clearAllExhibitsOnMap();
                    localSynchronizationMutex.release();
                }
            });

            localSynchronizationMutex.acquireUninterruptibly();

            getExhibitsForCurrentFloor();
            addAllExhibitsToMap();

            changeAndUpdateMutex.release();

            return null;
        }

        @Override
        protected void onPostExecute(Void aVoid) {
            super.onPostExecute(aVoid);

            Handler handler = new Handler();
            Runnable gc = new Runnable() {
                @Override
                public void run() {
                    System.gc();
                }
            };
            handler.postDelayed(gc, 1000);
        }
    }

    private MapBitmapProvider prepareBitmapProvider(Drawable drawable) {
        Bitmap map = null;

        if (drawable instanceof BitmapDrawable) {
            BitmapDrawable bitmapDrawable = (BitmapDrawable) drawable;
            map = bitmapDrawable.getBitmap();
        }

        if (map == null) {
            map = Bitmap.createBitmap(drawable.getIntrinsicWidth(),
                    drawable.getIntrinsicHeight(), Bitmap.Config.RGB_565);
            Canvas canvas = new Canvas(map);
            Drawable temporary = drawable.getCurrent();
            temporary.setBounds(0, 0, canvas.getWidth(), canvas.getHeight());
            temporary.draw(canvas);
        }

        MapBitmapProvider provider = new MapBitmapProvider();
        provider.prepareTiles(map);

        map.recycle();

        return provider;
    }


    // Exhibits handling:

    private void clearAllExhibitsOnMap() {
        for (HotSpot hs : mapState.hotSpotsForFloor) {
            tileView.removeHotSpot(hs);
        }
        mapState.hotSpotsForFloor.clear();

        mapState.exhibitsOverlay.removeAllViews();
    }

    private void getExhibitsForCurrentFloor() {
        mapState.exhibitsForFloor = DataHandler.getInstance().getExhibitsOfFloor(currentFloorNum);
    }

    private void addAllExhibitsToMap() {
        AutoResizeTextView artv;
        RelativeLayout.LayoutParams tvLP;
        ExhibitSpot es;
        Integer posX, posY, width, height;
        Drawable bcgDrawable;

        HotSpot.HotSpotTapListener exhibitsListener = new ExhibitTapListener();

        final ArrayList<Pair<View, RelativeLayout.LayoutParams>> viewArrayList = new ArrayList<>();
        for (Exhibit e: mapState.exhibitsForFloor) {
            posX = ScaleHelper.getDimensionWhenScaleApplied(e.getX(),
                    mapState.mapWidth,
                    mapState.currentMapSize.first);
            posY = ScaleHelper.getDimensionWhenScaleApplied(e.getY(),
                    mapState.mapWidth,
                    mapState.currentMapSize.second);
            width = ScaleHelper.getDimensionWhenScaleApplied(e.getWidth(),
                    mapState.mapWidth,
                    mapState.currentMapSize.first);
            height = ScaleHelper.getDimensionWhenScaleApplied(e.getHeight(),
                    mapState.mapHeight,
                    mapState.currentMapSize.second);

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
        Integer mapWidth, mapHeight;
        Pair<Integer, Integer> currentMapSize;

        List<Exhibit> exhibitsForFloor;
        List<HotSpot> hotSpotsForFloor;
        RelativeLayout exhibitsOverlay;
    }


    // Action listeners:

    private class ExhibitTapListener implements HotSpot.HotSpotTapListener {
        @Override
        public void onHotSpotTap(HotSpot hotSpot, int x, int y) {
            Integer id = ((ExhibitSpot) hotSpot).getExhibitId();
            Toast.makeText(getApplicationContext(), "Clicked exhibit with id: " + id.toString(), Toast.LENGTH_SHORT).show();
        }
    }

    private class FloorsSwitchListener implements CompoundButton.OnCheckedChangeListener {
        @Override
        public void onCheckedChanged(CompoundButton compoundButton, boolean b) {
            //floorsSwitch.setEnabled(false);
            floorsSwitch.setVisibility(View.INVISIBLE);

            if (b) {
                // Change to 2 floor
                currentFloorNum = DatabaseHelper.floor1Code;
            } else {
                // Change to 1 floor
                currentFloorNum = DatabaseHelper.floor0Code;
            }

            tileView.setBitmapProvider(null);
            new MapChangeFloor().execute(currentFloorNum);
        }
    }
}
