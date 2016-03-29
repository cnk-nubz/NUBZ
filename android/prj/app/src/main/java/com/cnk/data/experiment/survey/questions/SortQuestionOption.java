package com.cnk.data.experiment.survey.questions;

import com.cnk.data.ListObject;

public class SortQuestionOption implements ListObject {

    private int id;
    private String text;

    public SortQuestionOption(int id, String text) {
        this.id = id;
        this.text = text;
    }

    public int getId() {
        return id;
    }

    public String getText() {
        return text;
    }
}
