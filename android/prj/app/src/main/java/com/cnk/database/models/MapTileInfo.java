package com.cnk.database.models;

import com.cnk.data.map.Resolution;

public class MapTileInfo {
    private Integer floor;
    private Integer detailLevel;
    private Resolution tileSize;

    public MapTileInfo(Integer floor, Integer detailLevel, Resolution tileSize) {
        this.floor = floor;
        this.detailLevel = detailLevel;
        this.tileSize = tileSize;
    }

    public Integer getFloor() {
        return floor;
    }

    public Integer getDetailLevel() {
        return detailLevel;
    }

    public Resolution getTileSize() {
        return tileSize;
    }
}
