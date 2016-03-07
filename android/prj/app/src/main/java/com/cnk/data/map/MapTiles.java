package com.cnk.data.map;

import java.util.ArrayList;

public class MapTiles {

    ArrayList<ArrayList<String>> tilesFiles;
    private Resolution scaledSize;
    private Resolution tileSize;

    public MapTiles(Resolution scaledSize,
                    Resolution tileSize,
                    ArrayList<ArrayList<String>> tilesFiles) {
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

    public ArrayList<ArrayList<String>> getTilesFiles() {
        return tilesFiles;
    }

    public void setTilesFiles(ArrayList<ArrayList<String>> tilesFiles) {
        this.tilesFiles = tilesFiles;
    }
}
