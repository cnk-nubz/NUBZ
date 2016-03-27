package com.cnk.activities.map;

import android.content.Context;
import android.graphics.Rect;
import android.graphics.drawable.GradientDrawable;
import android.support.annotation.NonNull;
import android.util.Log;
import android.view.Gravity;
import android.view.View;
import android.view.ViewGroup;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;
import android.widget.TextView;

import com.cnk.R;
import com.cnk.data.map.MapData;
import com.cnk.data.map.Resolution;
import com.cnk.data.map.ZoomLevelInfo;
import com.cnk.database.models.Exhibit;
import com.cnk.notificators.Observable;
import com.cnk.ui.AutoResizeTextView;
import com.cnk.utilities.ColorHelper;
import com.qozix.tileview.TileView;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/*
    MapContainer assumes that map data won't change during its life
 */
public class MapContainer extends Observable<MapContainer.ExhibitClickListener> {
    public interface ExhibitClickListener {
        void notify(Exhibit exhibit);
    }

    private class MapView {
        final int floor;
        final TileView tileView;
        final ViewGroup exhibitsView;

        MapView(int floor, TileView tileView, ViewGroup exhibitsView) {
            this.floor = floor;
            this.tileView = tileView;
            this.exhibitsView = exhibitsView;
        }
    }

    private static final String LOG_TAG = "MapContainer";

    private ViewGroup parent;
    private Context context;

    private View noDataView;

    private MapView mapView;
    public Map<Integer, Exhibit> exhibits;
    private Map<Integer, ExhibitSpot> exhibitSpots;

    public MapContainer(@NonNull Context context, @NonNull ViewGroup parent) {
        this.context = context;
        this.parent = parent;
        exhibits = new HashMap<>();
        exhibitSpots = new HashMap<>();
    }

    public void destroy() {
        this.context = null;
        this.parent = null;
        this.observers = null;
    }

    public void switchToFloor(int floor) {
        if (MapData.getInstance().mapForFloorExists(floor)) {
            hideNoDataView();
            showMapView(floor);
        } else {
            hideMapView();
            showNoDataView();
        }
    }

    private void showNoDataView() {
        if (noDataView != null) {
            return;
        }

        int wrap = ViewGroup.LayoutParams.WRAP_CONTENT;

        LinearLayout noDataView = new LinearLayout(parent.getContext());
        noDataView.setOrientation(LinearLayout.VERTICAL);
        noDataView.setGravity(Gravity.CENTER_HORIZONTAL);

        TextView text = new TextView(parent.getContext());
        text.setText(context.getResources().getString(R.string.map_missing));
        text.setTextSize(20);
        noDataView.addView(text, new RelativeLayout.LayoutParams(wrap, wrap));

        RelativeLayout.LayoutParams params = new RelativeLayout.LayoutParams(wrap, wrap);
        params.addRule(RelativeLayout.CENTER_IN_PARENT);
        parent.addView(noDataView, params);

        this.noDataView = noDataView;
    }

    private void hideNoDataView() {
        if (noDataView != null) {
            parent.removeView(noDataView);
            noDataView = null;
        }
    }

    private void showMapView(int floor) {
        removeMapView();

        mapView = createMapView(floor);
        createZoomLevels(mapView);
        addExhibits(mapView);
        addMapViewTo(mapView, parent);
    }

    private void hideMapView() {
        removeMapView();
    }

    private void removeMapView() {
        if (mapView == null) {
            return;
        }

        removeAllExhibits(mapView);
        parent.removeView(mapView.tileView);
        mapView.tileView.destroy();
        mapView = null;
    }

    private MapView createMapView(int floor) {
        TileView tileView = new TileView(context);

        Resolution orgResolution = MapData.getInstance().getOriginalResolution(floor);
        Resolution maxZoom = MapData.getInstance().getMaxZoomResolution(floor);
        tileView.setSize(maxZoom.getWidth(), maxZoom.getHeight());
        tileView.defineBounds(0, 0, orgResolution.getWidth(), orgResolution.getHeight());

        tileView.setBitmapProvider(new MapBitmapProvider(floor));
        tileView.setTransitionsEnabled(false);
        tileView.setShouldRecycleBitmaps(false);
        tileView.setShouldScaleToFit(true);

        ViewGroup exhibitsView = new RelativeLayout(context);
        tileView.addScalingViewGroup(exhibitsView);

        return new MapView(floor, tileView, exhibitsView);
    }

    private void createZoomLevels(@NonNull MapView mapView) {
        int floor = mapView.floor;
        float maxWidth = MapData.getInstance().getMaxZoomResolution(floor).getWidth();

        List<ZoomLevelInfo> zoomLevels = MapData.getInstance().getZoomLevels(floor);
        for (int i = 0; i < zoomLevels.size(); i++) {
            ZoomLevelInfo zoomLevel = zoomLevels.get(i);
            float scale = zoomLevel.scaledSize.getWidth() / maxWidth;
            int tileWidth = zoomLevel.tileSize.getWidth();
            int tileHeight = zoomLevel.tileSize.getHeight();

            Log.i(LOG_TAG, "adding level: " + i + "; with scale: " + scale);
            mapView.tileView.addDetailLevel(scale, i, tileWidth, tileHeight);
        }

        float minScale = zoomLevels.get(0).scaledSize.getWidth() / maxWidth;
        mapView.tileView.setScaleLimits(minScale, 1.0f);
        mapView.tileView.setScale(minScale);
    }

    private void addMapViewTo(@NonNull MapView mapView, @NonNull ViewGroup parent) {
        int matchParent = RelativeLayout.LayoutParams.MATCH_PARENT;
        mapView.tileView.requestRender();
        parent.addView(mapView.tileView, new RelativeLayout.LayoutParams(matchParent, matchParent));
    }

    // quite naive way to update, but it may be enough
    public void updateExhibits(@NonNull List<Exhibit> updatedExhibits) {
        if (mapView != null) {
            removeAllExhibits(mapView);
        }

        for (Exhibit e : updatedExhibits) {
            Log.i(LOG_TAG, "updating exhibit " + e.getName());
            exhibits.put(e.getId(), e);
        }

        if (mapView != null) {
            addExhibits(mapView);
        }
    }

    private void removeAllExhibits(@NonNull MapView mapView) {
        for (ExhibitSpot spot : exhibitSpots.values()) {
            mapView.tileView.removeHotSpot(spot);
        }
        mapView.exhibitsView.removeAllViews();
        exhibitSpots.clear();
    }

    private void addExhibits(@NonNull MapView mapView) {
        for (Exhibit e : exhibits.values()) {
            if (e.getFloor() == null || e.getFloor() != mapView.floor) {
                continue;
            }

            int x = e.getX();
            int y = e.getY();
            int width = e.getWidth();
            int height = e.getHeight();

            ExhibitSpot spot = createExhibitSpot(e);
            spot.setTag(this);
            spot.set(new Rect(x, y, x + width, y + height));

            RelativeLayout.LayoutParams params = new RelativeLayout.LayoutParams(width, height);
            params.setMargins(x, y, 0, 0);

            mapView.exhibitsView.addView(spot.getTextView(), params);
            mapView.tileView.addHotSpot(spot);
            exhibitSpots.put(e.getId(), spot);

            spot.setHotSpotTapListener((hotSpot, clickedX, clickedY) -> {
                ExhibitSpot exhibitSpot = (ExhibitSpot) hotSpot;
                if (exhibits.containsKey(exhibitSpot.getExhibitId())) {
                    exhibitClicked(exhibits.get(exhibitSpot.getExhibitId()));
                } else {
                    Log.i(LOG_TAG, "ignored exhibit click");
                }
            });
        }
    }

    private ExhibitSpot createExhibitSpot(@NonNull Exhibit exhibit) {
        int borderSize = 5;

        AutoResizeTextView textView = new AutoResizeTextView(context);
        textView.setText(exhibit.getName());
        textView.setTextColor(ColorHelper.textColorForBackground(exhibit.getColor()));
        textView.setGravity(Gravity.CENTER);
        textView.setMinTextSize(2f);
        textView.setTextSize(100f);
        textView.setSingleLine(false);
        textView.setPadding(borderSize, borderSize, borderSize, borderSize);

        GradientDrawable bcgDrawable = new GradientDrawable();
        bcgDrawable.setColor(exhibit.getColor());
        bcgDrawable.setStroke(borderSize, ColorHelper.borderColorForBackground(exhibit.getColor()));
        textView.setBackground(bcgDrawable);

        return new ExhibitSpot(exhibit.getId(), textView);
    }

    private void exhibitClicked(@NonNull Exhibit exhibit) {
        Log.i(LOG_TAG, "Exhibit (id=" + exhibit.getId() + ") " + exhibit.getName() + " clicked");

        List<ExhibitClickListener> observersCopy = new ArrayList<>();
        for (ExhibitClickListener observer : observers.values()) {
            observersCopy.add(observer);
        }
        for (ExhibitClickListener observer : observersCopy) {
            observer.notify(exhibit);
        }
    }
}