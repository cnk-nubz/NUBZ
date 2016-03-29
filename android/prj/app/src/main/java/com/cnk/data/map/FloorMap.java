package com.cnk.data.map;

import java.util.ArrayList;

public class FloorMap {
    private Integer floor;
    private Resolution originalSize;
    private ArrayList<ZoomLevel> zoomLevels;

    public FloorMap(Integer floor, Resolution originalSize, ArrayList<ZoomLevel> zoomLevels) {
        this.floor = floor;
        this.originalSize = originalSize;
        this.zoomLevels = zoomLevels;
    }

    public Integer getFloor() {
        return floor;
    }

    public Resolution getOriginalSize() {
        return originalSize;
    }

    public ArrayList<ZoomLevel> getZoomLevels() {
        return zoomLevels;
    }

}
