package com.cnk.ui.questions;

import android.content.Context;

import com.cnk.data.experiment.answers.MultipleChoiceQuestionAnswer;
import com.cnk.data.experiment.answers.SimpleQuestionAnswer;
import com.cnk.data.experiment.answers.SortQuestionAnswer;
import com.cnk.data.experiment.questions.MultipleChoiceQuestion;
import com.cnk.data.experiment.questions.SimpleQuestion;
import com.cnk.data.experiment.questions.SortQuestion;

public class QuestionViewFactory {

    public static QuestionView createQuestionView(SimpleQuestion question, Context c, SimpleQuestionAnswer answer) {
        return new SimpleQuestionView(c, question, answer);
    }

    public static QuestionView createQuestionView(MultipleChoiceQuestion question, Context c, MultipleChoiceQuestionAnswer answer) {
        return new MultipleChoiceQuestionView(c, question, answer);
    }

    public static QuestionView createQuestionView(SortQuestion question, Context c, SortQuestionAnswer answer) {
        return new SortQuestionView(c, question, answer);
    }
}
