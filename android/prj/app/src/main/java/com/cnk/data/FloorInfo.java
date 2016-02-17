package com.cnk.data;

import com.cnk.database.models.DetailLevelRes;
import com.cnk.database.models.Exhibit;
import com.cnk.database.models.MapTileInfo;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class FloorInfo {
    private List<DetailLevelRes> detailLevelRes;
    private Map<Integer, Exhibit> exhibits;
    private List<MapTileInfo> mapTilesSizes;
    private Integer detailLevelsCount;
    private Lock exhibitLock;

    public FloorInfo() {
        mapTilesSizes = new ArrayList<>();
        detailLevelRes = new ArrayList<>();
        mapTilesSizes = new ArrayList<>();
        exhibits = new HashMap<>();
        detailLevelsCount = null;
        exhibitLock = new ReentrantLock(true);
    }

    public List<DetailLevelRes> getDetailLevelRes() {
        return detailLevelRes;
    }

    public void setDetailLevelRes(List<DetailLevelRes> detailLevelRes) {
        this.detailLevelRes = detailLevelRes;
    }

    public Map<Integer, Exhibit> getExhibits() {
        exhibitLock.lock();
        Map<Integer, Exhibit> ret = exhibits;
        exhibitLock.unlock();
        return ret;
    }

    public void setExhibits(Map<Integer, Exhibit> exhibits) {
        exhibitLock.lock();
        this.exhibits = exhibits;
        exhibitLock.unlock();
    }

    public List<MapTileInfo> getMapTilesSizes() {
        return mapTilesSizes;
    }

    public void setMapTilesSizes(List<MapTileInfo> mapTilesSizes) {
        this.mapTilesSizes = mapTilesSizes;
    }

    public Integer getDetailLevelsCount() {
        return detailLevelsCount;
    }

    public void setDetailLevelsCount(Integer detailLevelsCount) {
        this.detailLevelsCount = detailLevelsCount;
    }
}
