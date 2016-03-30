package com.cnk.data.experiment.survey.answers;

import java.io.Serializable;

public class SimpleQuestionAnswer implements Serializable {
    private String answer;

    public String getAnswer() {
        return answer;
    }

    public void setAnswer(String answer) {
        this.answer = answer;
        if (answer.length() == 0) {
            this.answer = null;
        }
    }

    @Override
    public String toString() {
        return "SimpleQuestionAnswer{" +
               "answer='" + answer + '\'' +
               '}';
    }
}
