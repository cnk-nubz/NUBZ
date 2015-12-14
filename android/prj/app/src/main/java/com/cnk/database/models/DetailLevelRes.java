package com.cnk.database.models;

import com.cnk.data.Resolution;

public class DetailLevelRes {
    private Integer floor;
    private Integer detailLevel;
    private Resolution originalRes;
    private Resolution scaledRes;

    public DetailLevelRes(Integer floor, Integer detailLevel, Resolution originalRes, Resolution scaledRes) {
        this.floor = floor;
        this.detailLevel = detailLevel;
        this.originalRes = originalRes;
        this.scaledRes = scaledRes;
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

    public Resolution getOriginalRes() {
        return originalRes;
    }

    public void setOriginalRes(Resolution originalRes) {
        this.originalRes = originalRes;
    }

    public Resolution getScaledRes() {
        return scaledRes;
    }

    public void setScaledRes(Resolution scaledRes) {
        this.scaledRes = scaledRes;
    }
}
