package com.cnk.data.experiment.questions;

public class SimpleQuestion {
    public enum AnswerType {
        TEXT,
        NUMBER;
    }

    private Integer id;
    private String question;
    private AnswerType answerType;

    public SimpleQuestion(Integer id, String question, AnswerType answerType) {
        this.id = id;
        this.question = question;
        this.answerType = answerType;
    }

    public Integer getId() {
        return id;
    }

    public String getQuestion() {
        return question;
    }

    public AnswerType getAnswerType() {
        return answerType;
    }
}
