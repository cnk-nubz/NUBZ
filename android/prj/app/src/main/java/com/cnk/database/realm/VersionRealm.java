package com.cnk.database.realm;

import io.realm.RealmObject;
import io.realm.annotations.PrimaryKey;

public class VersionRealm extends RealmObject {

    @PrimaryKey private String item;
    private Integer currentVersion;

    public String getItem() {
        return item;
    }

    public void setItem(String item) {
        this.item = item;
    }

    public Integer getCurrentVersion() {
        return currentVersion;
    }

    public void setCurrentVersion(Integer currentVersion) {
        this.currentVersion = currentVersion;
    }
}
