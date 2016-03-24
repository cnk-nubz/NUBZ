package com.cnk.ui.questions;

import android.content.Context;
import android.text.InputType;
import android.widget.EditText;
import android.widget.RelativeLayout;

import com.cnk.data.experiment.survey.answers.SimpleQuestionAnswer;
import com.cnk.data.experiment.survey.questions.SimpleQuestion;

public class SimpleQuestionView extends QuestionView {

    private static final float TEXT_SIZE = 25.0f;
    private EditText answerBox;
    private SimpleQuestionAnswer answer;

    public SimpleQuestionView(Context context,
                              SimpleQuestion question,
                              SimpleQuestionAnswer answer) {
        super(context, question.getQuestion());
        setUpAnswerBox(context, question.getAnswerType());
        this.answer = answer;
    }

    @Override
    public void saveAnswer() {
        answer.setAnswer(answerBox.getText().toString());
    }


    private void setUpAnswerBox(Context context, SimpleQuestion.AnswerType type) {
        answerBox = new EditText(context);
        answerBox.setHint("Wpisz odpowiedź");
        answerBox.setTextSize(TEXT_SIZE);
        LayoutParams
                params =
                new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.WRAP_CONTENT);
        params.addRule(RelativeLayout.CENTER_IN_PARENT);
        params.setMargins(50, 0, 50, 0);
        answerBox.setLayoutParams(params);
        if (type == SimpleQuestion.AnswerType.NUMBER) {
            answerBox.setInputType(InputType.TYPE_CLASS_NUMBER);
        } else {
            answerBox.setInputType(InputType.TYPE_CLASS_TEXT);
        }
        addView(answerBox);
    }
}
