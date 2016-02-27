package com.cnk.data.experiment;

import com.cnk.utilities.ListObject;

public class Action implements ListObject {

    private Integer id;
    private String text;

    public Action(Integer id, String text) {
        this.id = id;
        this.text = text;
    }

    @Override
    public Integer getId() {
        return id;
    }

    @Override
    public String getText() {
        return text;
    }
}
