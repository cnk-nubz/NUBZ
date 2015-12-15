package com.cnk.ui;

import android.app.Activity;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;
import android.os.AsyncTask;
import android.os.Bundle;
import android.view.Gravity;
import android.view.View;
import android.view.ViewGroup;
import android.widget.CompoundButton;
import android.widget.LinearLayout;
import android.widget.ProgressBar;
import android.widget.RelativeLayout;
import android.widget.Switch;
import android.widget.TextView;

import com.cnk.R;
import com.cnk.communication.NetworkHandler;
import com.cnk.data.DataHandler;
import com.qozix.tileview.TileView;
import com.qozix.tileview.detail.DetailLevel;
import com.qozix.tileview.graphics.BitmapProvider;

import java.util.Observable;
import java.util.Observer;
import java.util.concurrent.Semaphore;

public class MapActivity extends Activity implements Observer {

    private TileView tileView;
    private Semaphore updateMapDataSemaphore;
    private MapState mapState;
    private Switch floorsSwitch;
    private LinearLayout layoutLoading, layoutMapMissing;
    private Integer currentFloorNum;
    private NetworkHandler networkHandler;
    private Boolean fullyLoaded;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.map_activity_layout);
        RelativeLayout rlRootLayout = (RelativeLayout) findViewById(R.id.rlRootViewMapActivity);

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

        updateMapDataSemaphore = new Semaphore(1, true);

        mapState = new MapState();
        mapState.mapWidth = new Integer[2];
        mapState.mapHeight = new Integer[2];
        mapState.floorProvider = new BitmapProvider[2];
        mapState.firstRun = true;

        DataHandler.getInstance().addObserver(this);

        networkHandler = new NetworkHandler();

        new MapUpdateTask().execute();
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

    @Override
    protected void onPause() {
        super.onPause();
        tileView.pause();
    }

    @Override
    protected void onResume() {
        super.onResume();

        if (fullyLoaded) {
            tileView.resume();
            refreshMapAsFloor(currentFloorNum);
        }
        fullyLoaded = true;
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        tileView.setBitmapProvider(null);
        tileView.destroy();
        mapState.floorProvider = null;
        networkHandler.stopBgDownload();
    }

    @Override
    public void update(Observable observable, Object o) {
        if (mapState.mapVersion == null || DataHandler.getInstance().getMapVersion() > mapState.mapVersion) {
            mapState.mapVersion = DataHandler.getInstance().getMapVersion();
            new MapUpdateTask().execute();
        }
    }

    private void refreshMapAsFloor(int floor) {
        assert(floor == 0 || floor == 1);

        layoutMapMissing.setVisibility(View.INVISIBLE);
        tileView.setVisibility(View.INVISIBLE);
        layoutLoading.setVisibility(View.VISIBLE);

        if (mapState.floorProvider[floor] == null) {
            layoutLoading.setVisibility(View.INVISIBLE);
            layoutMapMissing.setVisibility(View.VISIBLE);
            return;
        }

        //stop rendering
        tileView.cancelRender();

        tileView.setSize(mapState.mapWidth[floor], mapState.mapHeight[floor]);
        tileView.onDetailLevelChanged(
                new DetailLevel(tileView.getDetailLevelManager(), 1.000f, 1f, 256, 256));
        tileView.defineBounds(0, 0, mapState.mapWidth[floor], mapState.mapHeight[floor]);
        tileView.setBitmapProvider(mapState.floorProvider[floor]);

        //start rendering again
        tileView.requestRender();

        tileView.setVisibility(View.VISIBLE);
        tileView.setShouldScaleToFit(true);
        tileView.setScale(0.02f);

        layoutLoading.setVisibility(View.INVISIBLE);
        tileView.setVisibility(View.VISIBLE);
    }

    private class FloorsSwitchListener implements CompoundButton.OnCheckedChangeListener {
        @Override
        public void onCheckedChanged(CompoundButton compoundButton, boolean b) {
            floorsSwitch.setEnabled(false);

            if (b) {
                // Change to 2 floor
                currentFloorNum = 1;
            } else {
                // Change to 1 floor
                currentFloorNum = 0;
            }

            refreshMapAsFloor(currentFloorNum);

            floorsSwitch.setEnabled(true);
        }
    }

    private class MapUpdateTask extends AsyncTask<Void, Void, Void> {
        private Drawable floorDrawable[];

        @Override
        protected void onPreExecute() {
            super.onPreExecute();

            updateMapDataSemaphore.acquireUninterruptibly();
            MapActivity.this.runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    floorsSwitch.setEnabled(false);
                }
            });

        }

        @Override
        protected Void doInBackground(Void... voids) {
            mapState.mapVersion = DataHandler.getInstance().getMapVersion();
            floorDrawable = new Drawable[2];

            for (int i = 0; i <= 1; i++) {
                floorDrawable[i] = DataHandler.getInstance().getFloorMap(i);

                if (floorDrawable[i] != null) {
                    mapState.floorProvider[i] = prepareBitmapProvider(floorDrawable[i]);
                    mapState.mapHeight[i] = floorDrawable[i].getIntrinsicHeight();
                    mapState.mapWidth[i] = floorDrawable[i].getIntrinsicWidth();
                }
            }

            return null;
        }

        @Override
        protected void onPostExecute(Void object) {
            super.onPostExecute(object);

            MapActivity.this.runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    refreshMapAsFloor(currentFloorNum);
                    floorsSwitch.setEnabled(true);
                }
            });

            networkHandler.startBgDownload();
            updateMapDataSemaphore.release();
        }
    }

    private class MapState {
        Integer mapVersion;
        Boolean firstRun;
        BitmapProvider floorProvider[];
        Integer mapWidth[], mapHeight[];
    }

    private MapBitmapProvider prepareBitmapProvider(Drawable drawable) {
        Bitmap map = null;

        if (drawable instanceof BitmapDrawable) {
            BitmapDrawable bitmapDrawable = (BitmapDrawable) drawable;
            if (bitmapDrawable.getBitmap() != null) {
                map = bitmapDrawable.getBitmap();
            }
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

        return provider;
    }
}
