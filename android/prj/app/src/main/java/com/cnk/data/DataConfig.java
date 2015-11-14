package com.cnk.data;

import java.io.Serializable;

public class DataConfig implements Serializable {
    private int mapVersion;
    private int exhibitsVersion;

    public DataConfig(int mapVersion, int exhibitsVersion) {
        this.mapVersion = mapVersion;
        this.exhibitsVersion = exhibitsVersion;
    }

    public void setMapVersion(int newVersion) {
        mapVersion = newVersion;
    }

    public int getMapVersion() {
        return mapVersion;
    }

    public int getExhibitsVersion() {
        return exhibitsVersion;
    }
}
