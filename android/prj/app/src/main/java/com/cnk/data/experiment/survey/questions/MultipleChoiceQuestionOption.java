package com.cnk.data.experiment.survey.questions;

import com.cnk.data.ListObject;

public class MultipleChoiceQuestionOption implements ListObject {

    private Integer id;
    private String text;

    public MultipleChoiceQuestionOption(Integer id, String text) {
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
