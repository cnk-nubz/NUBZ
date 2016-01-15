package com.cnk.database.realm;

import com.cnk.data.Resolution;

import io.realm.RealmObject;


public class MapTileInfoRealm extends RealmObject {
    private Integer floor;
    private Integer detailLevel;
    private Integer width;
    private Integer length;

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

    public Integer getWidth() {
        return width;
    }

    public void setWidth(Integer width) {
        this.width = width;
    }

    public Integer getLength() {
        return length;
    }

    public void setLength(Integer length) {
        this.length = length;
    }
}
