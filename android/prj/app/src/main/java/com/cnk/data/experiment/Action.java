package com.cnk.data.experiment;

public class Action {
    private Integer id;
    private String text;

    public Action(Integer id, String text) {
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
