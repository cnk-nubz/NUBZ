package com.cnk.data.map;

import com.cnk.database.models.MapTileInfo;
import com.cnk.database.models.ZoomLevelResolution;

import java.util.ArrayList;
import java.util.List;

public class FloorMapInfo {
    private List<ZoomLevelResolution> zoomLevelsResolutions;
    private List<MapTileInfo> mapTilesSizes;

    public FloorMapInfo() {
        mapTilesSizes = new ArrayList<>();
        zoomLevelsResolutions = new ArrayList<>();
        mapTilesSizes = new ArrayList<>();
    }

    public List<ZoomLevelResolution> getZoomLevelsResolutions() {
        return zoomLevelsResolutions;
    }

    public void setZoomLevelsResolutions(List<ZoomLevelResolution> zoomLevelsResolutions) {
        this.zoomLevelsResolutions = zoomLevelsResolutions;
    }

    public List<MapTileInfo> getMapTilesSizes() {
        return mapTilesSizes;
    }

    public void setMapTilesSizes(List<MapTileInfo> mapTilesSizes) {
        this.mapTilesSizes = mapTilesSizes;
    }
}
