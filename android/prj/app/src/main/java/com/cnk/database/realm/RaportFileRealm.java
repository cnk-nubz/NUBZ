package com.cnk.database.realm;

import io.realm.RealmObject;
import io.realm.annotations.PrimaryKey;

public class RaportFileRealm extends RealmObject {

    public static final String IN_PROGRESS = "IN_PRGRESS";
    public static final String SENT = "SENT";
    public static final String READY_TO_SEND = "READY_TO_SEND";

    @PrimaryKey
    private Integer id;
    private Integer serverId;
    private String fileName;
    // "READY_TO_SEND", "SENT", "IN_PROGRESS", enums are not supported
    private String state;

    public Integer getId() {
        return id;
    }

    public void setId(Integer id) {
        this.id = id;
    }

    public Integer getServerId() {
        return serverId;
    }

    public void setServerId(Integer serverId) {
        this.serverId = serverId;
    }

    public String getFileName() {
        return fileName;
    }

    public void setFileName(String fileName) {
        this.fileName = fileName;
    }

    public String getState() {
        return state;
    }

    public void setState(String state) {
        this.state = state;
    }
}
