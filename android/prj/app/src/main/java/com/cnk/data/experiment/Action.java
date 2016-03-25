package com.cnk.data.experiment;

import com.cnk.data.ListObject;

public class Action implements ListObject {

    private int id;
    private String text;

    public Action(int id, String text) {
        this.id = id;
        this.text = text;
    }

    @Override
    public int getId() {
        return id;
    }

    @Override
    public String getText() {
        return text;
    }
}
