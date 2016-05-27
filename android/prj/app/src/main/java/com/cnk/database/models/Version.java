package com.cnk.database.models;

public enum Version {
    EXHIBITS("Exhibits");

    private String code;

    Version(String code) {
        this.code = code;
    }

    @Override
    public String toString() {
        return code;
    }
}
