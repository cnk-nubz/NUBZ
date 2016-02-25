package com.cnk.data.experiment.answers;

import java.util.ArrayList;
import java.util.List;

public class AllAnswers {
    private List<SimpleQuestionAnswer> simpleAnswers;

    public AllAnswers() {
        simpleAnswers = new ArrayList<>();
    }

    public void addSimpleAnswer(SimpleQuestionAnswer answer) {
        simpleAnswers.add(answer);
    }
}
