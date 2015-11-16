package com.cnk.data;

import java.io.Serializable;

public class DataConfig implements Serializable {
    private Integer mapVersion;

    public DataConfig() {
        this.mapVersion = null;
    }

    public DataConfig(Integer mapVersion) {
        this.mapVersion = mapVersion;
    }

    public void setMapVersion(int newVersion) {
        mapVersion = newVersion;
    }

    public Integer getMapVersion() {
        return mapVersion;
    }
}
