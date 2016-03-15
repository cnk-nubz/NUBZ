package com.cnk.ui.models;

import android.widget.Chronometer;
import android.widget.RelativeLayout;

import com.cnk.data.Resolution;
import com.cnk.ui.AutoResizeTextView;
import com.cnk.ui.MapActivity;
import com.qozix.tileview.hotspots.HotSpot;

import java.util.ArrayList;

public class MapState {
    public MapState(MapActivity activity) {
        hotSpotsForFloor = new ArrayList<>();
        exhibitsOverlay = new RelativeLayout(activity);
    }

    public Resolution currentMapSize, originalMapSize;

    public ArrayList<HotSpot> hotSpotsForFloor;
    public RelativeLayout exhibitsOverlay;

    public AutoResizeTextView lastExhibitTextView;

    public Chronometer drawerChronometer;
}