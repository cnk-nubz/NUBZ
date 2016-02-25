package com.cnk.ui.questions;

import android.content.Context;
import android.graphics.Color;
import android.widget.RelativeLayout;
import android.widget.TextView;

public abstract class QuestionView extends RelativeLayout {
    private static final float NAME_SIZE = 40.0f;
    private static final Integer QUESTION_COLOR = Color.RED;

    public QuestionView(Context context) {
        super(context, null);
    }

    public QuestionView(Context context, String name) {
        super(context, null);
        setUpNameLabel(name, context);
        LayoutParams params = new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT);
        params.addRule(RelativeLayout.CENTER_HORIZONTAL);
        params.addRule(RelativeLayout.ALIGN_PARENT_TOP);
        setLayoutParams(params);
    }

    public abstract void saveAnswer();

    private void setUpNameLabel(String name, Context context) {
        TextView nameLabel = new TextView(context);
        LayoutParams params = new LayoutParams(LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT);
        params.addRule(RelativeLayout.CENTER_HORIZONTAL);
        nameLabel.setText(name);
        nameLabel.setTextSize(NAME_SIZE);
        nameLabel.setTextColor(QUESTION_COLOR);
        nameLabel.setLayoutParams(params);
        this.addView(nameLabel);
    }

}
