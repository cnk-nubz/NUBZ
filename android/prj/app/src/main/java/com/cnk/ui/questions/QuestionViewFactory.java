package com.cnk.ui.questions;

import android.content.Context;

import com.cnk.data.experiment.answers.SimpleQuestionAnswer;
import com.cnk.data.experiment.questions.SimpleQuestion;

public class QuestionViewFactory {

    public static QuestionView createQuestionView(SimpleQuestion question, Context c, SimpleQuestionAnswer answer) {
        SimpleQuestionView ret = new SimpleQuestionView(c, question.getName(), question.getAnswerType(), answer);
        return ret;
    }
}
