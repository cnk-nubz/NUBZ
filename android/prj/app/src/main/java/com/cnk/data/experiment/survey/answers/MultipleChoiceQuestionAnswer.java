package com.cnk.data.experiment.survey.answers;

import java.io.Serializable;
import java.util.List;

public class MultipleChoiceQuestionAnswer implements Serializable {
    private List<Integer> answer;

    public List<Integer> getAnswer() {
        return answer;
    }

    public void setAnswer(List<Integer> answer) {
        this.answer = answer;
        if (answer.size() == 0) {
            answer = null;
        }
    }

    @Override
    public String toString() {
        return "MultipleChoiceQuestionAnswer{" +
               "answer=" + answer +
               '}';
    }
}
