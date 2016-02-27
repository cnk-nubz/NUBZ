package com.cnk.data.experiment.answers;

import java.util.ArrayList;
import java.util.List;

public class SurveyAnswers {
    private List<SimpleQuestionAnswer> simpleAnswers;
    private List<MultipleChoiceQuestionAnswer> multipleChoiceAnswers;

    public SurveyAnswers() {
        simpleAnswers = new ArrayList<>();
        multipleChoiceAnswers = new ArrayList<>();
    }

    public void addSimpleAnswer(SimpleQuestionAnswer answer) {
        simpleAnswers.add(answer);
    }

    public void addMultipleChoiceAnswer(MultipleChoiceQuestionAnswer answer) {
        multipleChoiceAnswers.add(answer);
    }
}
