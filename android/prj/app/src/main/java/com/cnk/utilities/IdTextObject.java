package com.cnk.utilities;

/**
 * Created by maciek on 27/02/16.
 */
public class IdTextObject {
    protected Integer id;
    protected String text;

    public IdTextObject(Integer id, String text) {
        this.id = id;
        this.text = text;
    }

    public Integer getId() {
        return id;
    }

    public String getText() {
        return text;
    }
}
