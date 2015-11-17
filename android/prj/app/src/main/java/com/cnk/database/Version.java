package com.cnk.database;

import io.realm.RealmObject;
import io.realm.annotations.PrimaryKey;

public class Version extends RealmObject{

    @PrimaryKey
    private String item;

    private int currentVersion;

    public String getItem() {
        return item;
    }

    public void setItem(String item) {
        this.item = item;
    }

    public int getCurrentVersion() {
        return currentVersion;
    }

    public void setCurrentVersion(int currentVersion) {
        this.currentVersion = currentVersion;
    }
}
