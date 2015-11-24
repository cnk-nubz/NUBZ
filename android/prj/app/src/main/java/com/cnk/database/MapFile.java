package com.cnk.database;

public class MapFile {

    private Integer floor;
    private String mapFileLocation;

    public MapFile() {
    }

    public MapFile(Integer floor, String mapFileLocation) {
        this.floor = floor;
        this.mapFileLocation = mapFileLocation;
    }

    public Integer getFloor() {
        return floor;
    }
    public void setFloor(Integer floor) {
        this.floor = floor;
    }

    public String getMapFileLocation() {
        return mapFileLocation;
    }
    public void setMapFileLocation(String mapFileLocation) {
        this.mapFileLocation = mapFileLocation;
    }
}
