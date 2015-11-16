package com.cnk.data;

import java.io.Serializable;

public class DataConfig implements Serializable {
    private Integer mapVersion;
    private Integer exhibitsVersion;

    public DataConfig(Integer mapVersion, Integer exhibitsVersion) {
        this.mapVersion = mapVersion;
        this.exhibitsVersion = exhibitsVersion;
    }

    public void setMapVersion(int newVersion) {
        mapVersion = newVersion;
    }

    public Integer getMapVersion() {
        return mapVersion;
    }

    public int getExhibitsVersion() {
        return exhibitsVersion;
    }
}
