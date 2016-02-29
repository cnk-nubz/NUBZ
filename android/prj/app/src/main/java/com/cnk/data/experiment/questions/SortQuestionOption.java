package com.cnk.data.experiment.questions;

import com.cnk.utilities.ListObject;

public class SortQuestionOption implements ListObject {

    private Integer id;
    private String text;

    public SortQuestionOption(Integer id, String text) {
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
