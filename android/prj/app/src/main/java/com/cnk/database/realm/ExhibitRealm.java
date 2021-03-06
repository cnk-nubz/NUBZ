package com.cnk.database.realm;

import io.realm.RealmObject;
import io.realm.annotations.PrimaryKey;

public class ExhibitRealm extends RealmObject {
    @PrimaryKey private Integer id;

    private Integer x;
    private Integer y;
    private Integer width;
    private Integer height;
    private Integer floor;
    private Integer colorR;
    private Integer colorG;
    private Integer colorB;
    private String name;

    public Integer getId() {
        return id;
    }

    public void setId(Integer id) {
        this.id = id;
    }

    public Integer getX() {
        return x;
    }

    public void setX(Integer x) {
        this.x = x;
    }

    public Integer getY() {
        return y;
    }

    public void setY(Integer y) {
        this.y = y;
    }

    public Integer getWidth() {
        return width;
    }

    public void setWidth(Integer width) {
        this.width = width;
    }

    public Integer getHeight() {
        return height;
    }

    public void setHeight(Integer height) {
        this.height = height;
    }

    public Integer getFloor() {
        return floor;
    }

    public void setFloor(Integer floor) {
        this.floor = floor;
    }

    public Integer getColorR() {
        return colorR;
    }

    public void setColorR(Integer colorR) {
        this.colorR = colorR;
    }

    public Integer getColorG() {
        return colorG;
    }

    public void setColorG(Integer colorG) {
        this.colorG = colorG;
    }

    public Integer getColorB() {
        return colorB;
    }

    public void setColorB(Integer colorB) {
        this.colorB = colorB;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }
}
