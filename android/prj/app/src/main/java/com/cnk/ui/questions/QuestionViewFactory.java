package com.cnk.ui.questions;

import android.content.Context;

import com.cnk.data.experiment.survey.answers.MultipleChoiceQuestionAnswer;
import com.cnk.data.experiment.survey.answers.SimpleQuestionAnswer;
import com.cnk.data.experiment.survey.answers.SortQuestionAnswer;
import com.cnk.data.experiment.survey.questions.MultipleChoiceQuestion;
import com.cnk.data.experiment.survey.questions.SimpleQuestion;
import com.cnk.data.experiment.survey.questions.SortQuestion;

public class QuestionViewFactory {

    public static QuestionView createQuestionView(SimpleQuestion question,
                                                  Context c,
                                                  SimpleQuestionAnswer answer) {
        return new SimpleQuestionView(c, question, answer);
    }

    public static QuestionView createQuestionView(MultipleChoiceQuestion question,
                                                  Context c,
                                                  MultipleChoiceQuestionAnswer answer) {
        return new MultipleChoiceQuestionView(c, question, answer);
    }

    public static QuestionView createQuestionView(SortQuestion question,
                                                  Context c,
                                                  SortQuestionAnswer answer) {
        return new SortQuestionView(c, question, answer);
    }
}
