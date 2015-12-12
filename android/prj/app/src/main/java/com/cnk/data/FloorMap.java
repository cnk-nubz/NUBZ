package com.cnk.data;

import java.util.List;

public class FloorMap {

    private Resolution originalSize;
    private List<MapTiles> zoomLevels;

    public FloorMap(Resolution originalSize, List<MapTiles> zoomLevels) {
        this.originalSize = originalSize;
        this.zoomLevels = zoomLevels;
    }

    public Resolution getOriginalSize() {
        return originalSize;
    }

    public void setOriginalSize(Resolution originalSize) {
        this.originalSize = originalSize;
    }

    public List<MapTiles> getLevels() {
        return zoomLevels;
    }

    public void setLevels(List<MapTiles> levels) {
        this.zoomLevels = levels;
    }
}
