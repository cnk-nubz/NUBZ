package com.cnk.data.experiment.answers;

import java.io.Serializable;

public class SimpleQuestionAnswer implements Serializable {
    private String answer;

    public String getAnswer() {
        return answer;
    }

    public void setAnswer(String answer) {
        this.answer = answer;
    }

    @Override
    public String toString() {
        return "SimpleQuestionAnswer{" +
               "answer='" + answer + '\'' +
               '}';
    }
}
