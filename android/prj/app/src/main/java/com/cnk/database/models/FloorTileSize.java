package com.cnk.database.models;

import com.cnk.data.Resolution;

public class FloorTileSize {
    private Integer floor;
    private Integer detailLevel;
    private Resolution tileSize;

    public FloorTileSize(Integer floor, Integer detailLevel, Resolution tileSize) {
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
