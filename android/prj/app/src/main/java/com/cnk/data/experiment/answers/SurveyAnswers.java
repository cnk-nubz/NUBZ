package com.cnk.data.experiment.answers;

import java.util.ArrayList;
import java.util.List;

public class SurveyAnswers {
    private List<SimpleQuestionAnswer> simpleAnswers;
    private List<MultipleChoiceQuestionAnswer> multipleChoiceAnswers;
    private List<SortQuestionAnswer> sortQuestionAnswers;

    public SurveyAnswers() {
        simpleAnswers = new ArrayList<>();
        multipleChoiceAnswers = new ArrayList<>();
        sortQuestionAnswers = new ArrayList<>();
    }

    public void addSimpleAnswer(SimpleQuestionAnswer answer) {
        simpleAnswers.add(answer);
    }

    public void addMultipleChoiceAnswer(MultipleChoiceQuestionAnswer answer) {
        multipleChoiceAnswers.add(answer);
    }

    public void addSortQuestionAnswer(SortQuestionAnswer answer) {
        sortQuestionAnswers.add(answer);
    }

    public List<SimpleQuestionAnswer> getSimpleAnswers() {
        return simpleAnswers;
    }

    public List<MultipleChoiceQuestionAnswer> getMultipleChoiceAnswers() {
        return multipleChoiceAnswers;
    }

    public List<SortQuestionAnswer> getSortQuestionAnswers() {
        return sortQuestionAnswers;
    }
}
