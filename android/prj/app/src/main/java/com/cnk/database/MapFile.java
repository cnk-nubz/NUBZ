package com.cnk.database;

import io.realm.RealmObject;
import io.realm.annotations.PrimaryKey;

public class MapFile extends RealmObject{

    @PrimaryKey
    private int floor;

    private String mapFileLocation;

    public int getFloor() {
        return floor;
    }

    public void setFloor(int floor) {
        this.floor = floor;
    }

    public String getMapFileLocation() {
        return mapFileLocation;
    }

    public void setMapFileLocation(String mapFileLocation) {
        this.mapFileLocation = mapFileLocation;
    }
}
