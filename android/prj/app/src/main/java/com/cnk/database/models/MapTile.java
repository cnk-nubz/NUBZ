package com.cnk.database.models;

public class MapTile {

    private Integer floor;
    private Integer detailLevel;
    private Integer rowNumber;
    private Integer columnNumber;
    private String mapTileLocation;

    public MapTile() {
    }

    public MapTile(Integer floor, Integer detailLevel, Integer rowNumber, Integer columnNumber, String mapTileLocation) {
        this.floor = floor;
        this.detailLevel = detailLevel;
        this.rowNumber = rowNumber;
        this.columnNumber = columnNumber;
        this.mapTileLocation = mapTileLocation;
    }

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
