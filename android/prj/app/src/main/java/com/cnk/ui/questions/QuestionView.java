package com.cnk.ui.questions;

import android.content.Context;
import android.graphics.Color;
import android.graphics.Typeface;
import android.view.Gravity;
import android.widget.RelativeLayout;
import android.widget.TextView;

import com.cnk.R;

public abstract class QuestionView extends RelativeLayout {
    private static final float NAME_SIZE = 40.0f;
    private static final Typeface QUESTION_TYPEFACE = Typeface.SERIF;
    private static final Integer QUESTION_COLOR = 0xFF6AC0D9;
    private static final float QUESTION_SHADOW_RANGE = 5f;
    private static final float QUESTION_SHADOW_DX = 3f;
    private static final float QUESTION_SHADOW_DY = 3f;
    private static final Integer QUESTION_SHADOW_COLOR = Color.LTGRAY;

    public QuestionView(Context context) {
        super(context, null);
    }

    public QuestionView(Context context, String name) {
        super(context, null);
        setUpNameLabel(name, context);
        LayoutParams
                params =
                new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT);
        params.addRule(RelativeLayout.CENTER_HORIZONTAL);
        params.addRule(RelativeLayout.ALIGN_PARENT_TOP);
        params.addRule(ABOVE, R.id.counterLabel);
        setLayoutParams(params);
    }

    public abstract void saveAnswer();

    private void setUpNameLabel(String name, Context context) {
        TextView nameLabel = new TextView(context);
        LayoutParams
                params =
                new LayoutParams(LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT);
        params.addRule(RelativeLayout.CENTER_HORIZONTAL);
        nameLabel.setText(name);
        nameLabel.setTextSize(NAME_SIZE);
        nameLabel.setTypeface(QUESTION_TYPEFACE);
        nameLabel.setTextColor(QUESTION_COLOR);
        nameLabel.setShadowLayer(QUESTION_SHADOW_RANGE,
                                 QUESTION_SHADOW_DX,
                                 QUESTION_SHADOW_DY,
                                 QUESTION_SHADOW_COLOR);
        nameLabel.setGravity(Gravity.CENTER_HORIZONTAL);
        nameLabel.setLayoutParams(params);
        nameLabel.setId(R.id.questionNameLabelId);
        this.addView(nameLabel);
    }
}
