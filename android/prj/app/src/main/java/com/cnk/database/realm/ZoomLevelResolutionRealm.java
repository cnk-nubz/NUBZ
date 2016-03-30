package com.cnk.database.realm;


import io.realm.RealmObject;

public class ZoomLevelResolutionRealm extends RealmObject {
    private Integer floor;
    private Integer zoomLevel;
    private Integer originalWidth;
    private Integer originalHeight;
    private Integer scaledWidth;
    private Integer scaledHeight;

    public Integer getFloor() {
        return floor;
    }

    public void setFloor(Integer floor) {
        this.floor = floor;
    }

    public Integer getZoomLevel() {
        return zoomLevel;
    }

    public void setZoomLevel(Integer zoomLevel) {
        this.zoomLevel = zoomLevel;
    }

    public Integer getOriginalWidth() {
        return originalWidth;
    }

    public void setOriginalWidth(Integer originalWidth) {
        this.originalWidth = originalWidth;
    }

    public Integer getOriginalHeight() {
        return originalHeight;
    }

    public void setOriginalHeight(Integer originalHeight) {
        this.originalHeight = originalHeight;
    }

    public Integer getScaledWidth() {
        return scaledWidth;
    }

    public void setScaledWidth(Integer scaledWidth) {
        this.scaledWidth = scaledWidth;
    }

    public Integer getScaledHeight() {
        return scaledHeight;
    }

    public void setScaledHeight(Integer scaledHeight) {
        this.scaledHeight = scaledHeight;
    }
}
