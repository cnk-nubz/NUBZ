package com.cnk.data.map;

import com.cnk.database.models.DetailLevelRes;
import com.cnk.database.models.MapTileInfo;

import java.util.ArrayList;
import java.util.List;

public class FloorMapInfo {
    private List<DetailLevelRes> detailLevelRes;
    private List<MapTileInfo> mapTilesSizes;
    private Integer detailLevelsCount;

    public FloorMapInfo() {
        mapTilesSizes = new ArrayList<>();
        detailLevelRes = new ArrayList<>();
        mapTilesSizes = new ArrayList<>();
        detailLevelsCount = null;
    }

    public List<DetailLevelRes> getDetailLevelRes() {
        return detailLevelRes;
    }

    public void setDetailLevelRes(List<DetailLevelRes> detailLevelRes) {
        this.detailLevelRes = detailLevelRes;
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
