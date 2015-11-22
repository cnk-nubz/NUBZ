package com.cnk.database;

import io.realm.RealmObject;
import io.realm.annotations.PrimaryKey;

public class MapFileRealm extends RealmObject {

    @PrimaryKey
    private Integer floor;
    private String mapFileLocation;

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
