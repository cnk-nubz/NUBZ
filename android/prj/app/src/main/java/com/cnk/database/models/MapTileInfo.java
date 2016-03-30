package com.cnk.database.models;

import com.cnk.data.map.Resolution;
import com.cnk.database.realm.MapTileInfoRealm;

public class MapTileInfo implements Realmable {
    private Integer floor;
    private Resolution tileSize;
    private Integer zoomLevel;

    public MapTileInfo(MapTileInfoRealm realm) {
        this(realm.getFloor(),
             realm.getZoomLevel(),
             new Resolution(realm.getWidth(), realm.getHeight()));
    }

    public MapTileInfo(Integer floor, Integer zoomLevel, Resolution tileSize) {
        this.floor = floor;
        this.tileSize = tileSize;
        this.zoomLevel = zoomLevel;
    }

    @Override
    public MapTileInfoRealm toRealm() {
        MapTileInfoRealm res = new MapTileInfoRealm();
        res.setFloor(floor);
        res.setZoomLevel(zoomLevel);
        res.setWidth(tileSize.getWidth());
        res.setHeight(tileSize.getHeight());
        return res;
    }

    public Resolution getTileSize() {
        return tileSize;
    }
}
