package com.cnk.database.models;

public class Exhibit {
    private Integer id;
    private Integer x;
    private Integer y;
    private Integer width;
    private Integer height;
    private Integer floor;
    private String name;

    public Exhibit() {
    }

    public Exhibit(Integer id, Integer x, Integer y, Integer width, Integer height, Integer floor, String name) {
        this.id = id;
        this.x = x;
        this.y = y;
        this.width = width;
        this.height = height;
        this.floor = floor;
        this.name = name;
    }

    public Exhibit(Integer id, com.cnk.communication.thrift.Exhibit exhibitFromServer) {
        this.id = id;
        this.name = exhibitFromServer.getName();
        if (exhibitFromServer.getMapFrame() != null) {
            this.x = exhibitFromServer.getMapFrame().getFrame().getX();
            this.y = exhibitFromServer.getMapFrame().getFrame().getY();
            this.width = exhibitFromServer.getMapFrame().getFrame().getSize().getWidth();
            this.height = exhibitFromServer.getMapFrame().getFrame().getSize().getHeight();
            this.floor = exhibitFromServer.getMapFrame().getFloor();
        }
    }

    public Integer getId() {
        return id;
    }

    public Integer getX() {
        return x;
    }

    public Integer getY() {
        return y;
    }

    public Integer getWidth() {
        return width;
    }

    public Integer getHeight() {
        return height;
    }

    public Integer getFloor() {
        return floor;
    }

    public String getName() {
        return name;
    }
}
