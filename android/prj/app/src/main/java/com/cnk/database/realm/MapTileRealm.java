package com.cnk.database.realm;

import io.realm.RealmObject;

public class MapTileRealm extends RealmObject {

    private Integer floor;
    private Integer detailLevel;
    private Integer rowNumber;
    private Integer columnNumber;
    private String mapTileLocation;

    public Integer getFloor() {
        return floor;
    }
    public void setFloor(Integer floor) {
        this.floor = floor;
    }

    public Integer getDetailLevel() {
        return detailLevel;
    }
    public void setDetailLevel(Integer detailLevel) {
        this.detailLevel = detailLevel;
    }

    public Integer getRowNumber() {
        return rowNumber;
    }
    public void setRowNumber(Integer rowNumber) {
        this.rowNumber = rowNumber;
    }

    public Integer getColumnNumber() {
        return columnNumber;
    }
    public void setColumnNumber(Integer columnNumber) {
        this.columnNumber = columnNumber;
    }

    public String getMapTileLocation() {
        return mapTileLocation;
    }
    public void setMapTileLocation(String mapTileLocation) {
        this.mapTileLocation = mapTileLocation;
    }
}
