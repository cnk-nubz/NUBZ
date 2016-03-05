package com.cnk.database.realm;

import io.realm.RealmObject;
import io.realm.annotations.PrimaryKey;

public class FloorDetailLevelsRealm extends RealmObject {

    @PrimaryKey
    private Integer floorNo;
    private Integer detailLevels;

    public Integer getFloorNo() {
        return floorNo;
    }

    public void setFloorNo(Integer floorNo) {
        this.floorNo = floorNo;
    }

    public Integer getDetailLevels() {
        return detailLevels;
    }

    public void setDetailLevels(Integer detailLevels) {
        this.detailLevels = detailLevels;
    }
}
