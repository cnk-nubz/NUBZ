package com.cnk.data;

import java.util.List;

public class MapTiles {

    private Resolution scaledSize;
    private Resolution tileSize;
    List<List<String>> tilesUrls;

    public MapTiles(Resolution scaledSize, Resolution tileSize, List<List<String>> tilesUrls) {
        this.scaledSize = scaledSize;
        this.tileSize = tileSize;
        this.tilesUrls = tilesUrls;
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

    public List<List<String>> getTilesUrls() {
        return tilesUrls;
    }

    public void setTilesUrls(List<List<String>> tilesUrls) {
        this.tilesUrls = tilesUrls;
    }
}
