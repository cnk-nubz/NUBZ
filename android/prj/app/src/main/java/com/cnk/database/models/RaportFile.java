package com.cnk.database.models;

import com.cnk.database.realm.RaportFileRealm;

public class RaportFile {

    private Integer id;
    private Integer serverId;
    private String fileName;

    public RaportFile(RaportFileRealm realmFile) {
        this(realmFile.getId(),
             realmFile.getServerId(),
             realmFile.getFileName());
    }

    public RaportFile(Integer id, Integer serverId, String fileName) {
        this.id = id;
        this.serverId = serverId;
        this.fileName = fileName;
    }

    public Integer getId() {
        return id;
    }

    public Integer getServerId() {
        return serverId;
    }

    public String getFileName() {
        return fileName;
    }
}
