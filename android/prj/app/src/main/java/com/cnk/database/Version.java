package com.cnk.database;

import io.realm.RealmObject;
import io.realm.annotations.PrimaryKey;

public class Version extends RealmObject{

    public enum Item {
        MAP("Map") {
            @Override
            public String toString() {
                return super.toString();
            }
        }, UNKNOWN("Unknown") {
            @Override
            public String toString() {
                return super.toString();
            }
        };
        private String code;

        private Item(String code) {
            this.code = code;
        }

        @Override
        public String toString() {
            return code;
        }
    }

    @PrimaryKey
    private String item;

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
