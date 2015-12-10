package com.cnk.database;

public class RaportFile {

    private Integer id;
    private Integer serverId;
    private String fileName;
    // "READY_TO_SEND", "SENT", "IN_PROGRESS", enums are not supported
    private String state;

    public RaportFile(Integer id, Integer serverId, String fileName, String state) {
        this.id = id;
        this.serverId = serverId;
        this.fileName = fileName;
        this.state = state;
    }

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
