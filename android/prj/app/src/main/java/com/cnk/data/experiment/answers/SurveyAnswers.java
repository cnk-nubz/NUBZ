package com.cnk.data.experiment.answers;

import java.util.ArrayList;
import java.util.List;

public class SurveyAnswers {
    private List<SimpleQuestionAnswer> simpleAnswers;

    public SurveyAnswers() {
        simpleAnswers = new ArrayList<>();
    }

    public void addSimpleAnswer(SimpleQuestionAnswer answer) {
        simpleAnswers.add(answer);
    }
}
