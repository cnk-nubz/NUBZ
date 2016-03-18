package com.cnk.data.experiment.survey.questions;

import java.util.List;

public class SortQuestion {
    private Integer id;
    private String question;
    private List<SortQuestionOption> options;

    public SortQuestion(Integer id, String question, List<SortQuestionOption> options) {
        this.id = id;
        this.question = question;
        this.options = options;
    }

    public Integer getId() {
        return id;
    }

    public String getQuestion() {
        return question;
    }

    public List<SortQuestionOption> getOptions() {
        return options;
    }
}
