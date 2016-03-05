package com.cnk.data.experiment.questions;

import java.util.List;

public class MultipleChoiceQuestion {
    private Integer id;
    private String question;
    private boolean singleAnswer;
    private List<MultipleChoiceQuestionOption> options;

    public MultipleChoiceQuestion(Integer id, String question, boolean singleAnswer, List<MultipleChoiceQuestionOption> options) {
        this.id = id;
        this.question = question;
        this.singleAnswer = singleAnswer;
        this.options = options;
    }

    public Integer getId() {
        return id;
    }

    public String getQuestion() {
        return question;
    }

    public boolean isSingleAnswer() {
        return singleAnswer;
    }

    public List<MultipleChoiceQuestionOption> getOptions() {
        return options;
    }
}
