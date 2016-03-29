package com.cnk.database.models;

import com.cnk.data.map.Resolution;
import com.cnk.database.realm.ZoomLevelResolutionRealm;

public class ZoomLevelResolution implements Realmable {
    private Integer floor;
    private Integer zoomLevel;
    private Resolution originalRes;
    private Resolution scaledRes;

    public ZoomLevelResolution(ZoomLevelResolutionRealm realm) {
        floor = realm.getFloor();
        zoomLevel = realm.getZoomLevel();
        originalRes = new Resolution(realm.getOriginalWidth(), realm.getOriginalHeight());
        scaledRes = new Resolution(realm.getScaledWidth(), realm.getScaledHeight());
    }

    public ZoomLevelResolution(Integer floor,
                               Integer zoomLevel,
                               Resolution originalRes,
                               Resolution scaledRes) {
        this.floor = floor;
        this.zoomLevel = zoomLevel;
        this.originalRes = originalRes;
        this.scaledRes = scaledRes;
    }

    @Override
    public ZoomLevelResolutionRealm toRealm() {
        ZoomLevelResolutionRealm res = new ZoomLevelResolutionRealm();
        res.setFloor(floor);
        res.setZoomLevel(zoomLevel);
        res.setOriginalHeight(originalRes.getHeight());
        res.setOriginalWidth(originalRes.getWidth());
        res.setScaledHeight(scaledRes.getHeight());
        res.setScaledWidth(scaledRes.getWidth());
        return res;
    }

    public Resolution getOriginalRes() {
        return originalRes;
    }

    public Resolution getScaledRes() {
        return scaledRes;
    }
}
