package com.cnk.ui.questions;

import android.content.Context;
import android.text.InputType;
import android.widget.EditText;
import android.widget.RelativeLayout;

import com.cnk.data.experiment.SimpleQuestion;
import com.cnk.data.experiment.SimpleQuestionAnswer;

public class SimpleQuestionView extends QuestionView {

    private static final float TEXT_SIZE = 25.0f;
    private EditText answerBox;

    public SimpleQuestionView(Context context, String name, SimpleQuestion.AnswerType type) {
        super(context, name);
        setUpAnswerBox(context, type);
    }

    public SimpleQuestionAnswer getAnswer() {
        return new SimpleQuestionAnswer(answerBox.getText().toString());
    }

    private void setUpAnswerBox(Context context, SimpleQuestion.AnswerType type) {
        answerBox = new EditText(context);
        answerBox.setHint("Wpisz odpowiedź");
        answerBox.setTextSize(TEXT_SIZE);
        LayoutParams params = new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.WRAP_CONTENT);
        params.addRule(RelativeLayout.CENTER_IN_PARENT);
        answerBox.setLayoutParams(params);
        if (type == SimpleQuestion.AnswerType.NUMBER) {
            answerBox.setInputType(InputType.TYPE_CLASS_NUMBER);
        } else {
            answerBox.setInputType(InputType.TYPE_CLASS_TEXT);
        }
        addView(answerBox);
    }
}
