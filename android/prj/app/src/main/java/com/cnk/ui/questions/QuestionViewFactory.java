package com.cnk.ui.questions;

import android.content.Context;

import com.cnk.data.experiment.SimpleQuestion;
import com.cnk.data.experiment.Survey;

import java.util.ArrayList;
import java.util.List;

public class QuestionViewFactory {

    public static List<QuestionView> createQuestionViews(Survey survey, Context c) {
        List<QuestionView> views = new ArrayList<>();
        while (survey.getRemainingQuestionsCount() > 0) {
            Survey.QuestionType currentType = survey.popNextQuestionType();
            if (currentType == Survey.QuestionType.SIMPLE) {
                SimpleQuestion currentQuestion = survey.popNextSimpleQuestion();
                views.add(new SimpleQuestionView(c, currentQuestion.getName(), currentQuestion.getAnswerType()));
            } else if (currentType == Survey.QuestionType.MULTIPLE_CHOICE) {

            }
        }
        return views;
    }
}
