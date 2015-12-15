package com.cnk.data;

import java.util.List;

public class MapTiles {

    private Resolution scaledSize;
    private Resolution tileSize;
    List<List<String>> tilesFiles;

    public MapTiles(Resolution scaledSize, Resolution tileSize, List<List<String>> tilesFiles) {
        this.scaledSize = scaledSize;
        this.tileSize = tileSize;
        this.tilesFiles = tilesFiles;
    }

    public Resolution getScaledSize() {
        return scaledSize;
    }

    public void setScaledSize(Resolution scaledSize) {
        this.scaledSize = scaledSize;
    }

    public Resolution getTileSize() {
        return tileSize;
    }

    public void setTileSize(Resolution tileSize) {
        this.tileSize = tileSize;
    }

    public List<List<String>> getTilesFiles() {
        return tilesFiles;
    }

    public void setTilesFiles(List<List<String>> tilesFiles) {
        this.tilesFiles = tilesFiles;
    }
}
