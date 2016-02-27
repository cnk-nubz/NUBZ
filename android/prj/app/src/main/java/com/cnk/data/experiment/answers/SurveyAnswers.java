package com.cnk.data.experiment.answers;

import java.util.ArrayList;
import java.util.List;

public class SurveyAnswers {
    private List<SimpleQuestionAnswer> simpleAnswers;
    private List<MultipleChoiceQuestionAnswer> multipleChoiceanswers;

    public SurveyAnswers() {
        simpleAnswers = new ArrayList<>();
        multipleChoiceanswers = new ArrayList<>();
    }

    public void addSimpleAnswer(SimpleQuestionAnswer answer) {
        simpleAnswers.add(answer);
    }

    public void addMultipleChoiceAnswer(MultipleChoiceQuestionAnswer answer) {
        multipleChoiceanswers.add(answer);
    }
}
