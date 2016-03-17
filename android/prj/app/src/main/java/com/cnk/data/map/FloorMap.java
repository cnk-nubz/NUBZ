package com.cnk.data.map;

import java.util.ArrayList;

public class FloorMap {

    private Resolution originalSize;
    private ArrayList<MapTiles> zoomLevels;

    public FloorMap(Resolution originalSize, ArrayList<MapTiles> zoomLevels) {
        this.originalSize = originalSize;
        this.zoomLevels = zoomLevels;
    }

    public Resolution getOriginalSize() {
        return originalSize;
    }

    public ArrayList<MapTiles> getLevels() {
        return zoomLevels;
    }

}
