package com.cnk.database.models;

public class Version {
    public enum Item {
        MAP("Map"),
        EXHIBITS("Exhibits"),
        UNKNOWN("Unknown");

        private String code;

        private Item(String code) {
            this.code = code;
        }

        @Override
        public String toString() {
            return code;
        }

        public static Item fromString(String code) {
            if (code != null) {
                for (Item i : Item.values()) {
                    if (code.equals(i.code)) {
                        return i;
                    }
                }
            }
            return Item.UNKNOWN;
        }
    }

    private Item item;
    private Integer currentVersion;

    public Version() {
    }

    public Version(Item item, Integer currentVersion) {
        this.item = item;
        this.currentVersion = currentVersion;
    }

    public Item getItem() {
        return item;
    }
    public void setItem(Item item) {
        this.item = item;
    }

    public Integer getCurrentVersion() {
        return currentVersion;
    }
    public void setCurrentVersion(Integer currentVersion) {
        this.currentVersion = currentVersion;
    }
}
