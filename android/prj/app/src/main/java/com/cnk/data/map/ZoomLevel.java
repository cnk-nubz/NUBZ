package com.cnk.data.map;

import java.util.ArrayList;

public class ZoomLevel {

    ArrayList<ArrayList<String>> tilesFiles;
    private Resolution scaledSize;
    private Resolution tileSize;

    public ZoomLevel(Resolution scaledSize,
                     Resolution tileSize,
                     ArrayList<ArrayList<String>> tilesFiles) {
        this.scaledSize = scaledSize;
        this.tileSize = tileSize;
        this.tilesFiles = tilesFiles;
    }

    public Resolution getScaledSize() {
        return scaledSize;
    }


    public Resolution getTileSize() {
        return tileSize;
    }


    public ArrayList<ArrayList<String>> getTilesFiles() {
        return tilesFiles;
    }

}
