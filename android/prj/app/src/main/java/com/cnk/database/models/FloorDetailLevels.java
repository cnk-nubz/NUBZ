package com.cnk.database.models;

public class FloorDetailLevels {

    private Integer floorNo;
    private Integer detailLevels;

    public FloorDetailLevels(Integer floorNo, Integer detailLevels) {
        this.floorNo = floorNo;
        this.detailLevels = detailLevels;
    }

    public Integer getFloorNo() {
        return floorNo;
    }

    public Integer getDetailLevels() {
        return detailLevels;
    }
}
