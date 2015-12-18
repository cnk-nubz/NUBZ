package com.cnk.data;

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

    public void setOriginalSize(Resolution originalSize) {
        this.originalSize = originalSize;
    }

    public ArrayList<MapTiles> getLevels() {
        return zoomLevels;
    }

    public void setLevels(ArrayList<MapTiles> levels) {
        this.zoomLevels = levels;
    }
}
