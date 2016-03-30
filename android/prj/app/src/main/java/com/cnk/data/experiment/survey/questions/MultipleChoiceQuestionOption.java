package com.cnk.data.experiment.survey.questions;

import com.cnk.data.ListObject;

public class MultipleChoiceQuestionOption implements ListObject {

    private int id;
    private String text;

    public MultipleChoiceQuestionOption(int id, String text) {
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
