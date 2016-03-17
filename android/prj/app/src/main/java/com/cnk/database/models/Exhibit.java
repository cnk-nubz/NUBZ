package com.cnk.database.models;

import com.cnk.communication.thrift.Frame;
import com.cnk.database.realm.ExhibitRealm;

public class Exhibit implements Realmable {
    private Integer id;
    private Integer x;
    private Integer y;
    private Integer width;
    private Integer height;
    private Integer floor;
    private String name;

    public Exhibit(ExhibitRealm er) {
        this(er.getId(),
             er.getX(),
             er.getY(),
             er.getWidth(),
             er.getHeight(),
             er.getFloor(),
             er.getName());
    }

    public Exhibit(Integer id,
                   Integer x,
                   Integer y,
                   Integer width,
                   Integer height,
                   Integer floor,
                   String name) {
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
        name = exhibitFromServer.getName();
        if (exhibitFromServer.getMapFrame() != null) {
            Frame frame = exhibitFromServer.getMapFrame().getFrame();
            x = frame.getX();
            y = frame.getY();
            width = frame.getSize().getWidth();
            height = frame.getSize().getHeight();
            floor = exhibitFromServer.getMapFrame().getFloor();
        }
    }

    @Override
    public ExhibitRealm toRealm() {
        ExhibitRealm er = new ExhibitRealm();
        er.setId(id);
        er.setX(x);
        er.setY(y);
        er.setWidth(width);
        er.setHeight(height);
        er.setFloor(floor);
        er.setName(name);
        return er;
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
