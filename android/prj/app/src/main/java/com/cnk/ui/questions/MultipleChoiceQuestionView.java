package com.cnk.ui.questions;

import android.content.Context;
import android.widget.ListView;
import android.widget.RelativeLayout;
import android.widget.TextView;

import com.cnk.R;
import com.cnk.data.experiment.answers.MultipleChoiceQuestionAnswer;
import com.cnk.data.experiment.questions.MultipleChoiceQuestion;
import com.cnk.data.experiment.questions.MultipleChoiceQuestionOption;
import com.cnk.ui.adapters.SelectListAdapter;

public class MultipleChoiceQuestionView extends QuestionView {

    private static final float HEADER_SIZE = 25.0f;
    SelectListAdapter<MultipleChoiceQuestionOption> adapter;
    private MultipleChoiceQuestionAnswer answer;

    public MultipleChoiceQuestionView(Context c,
                                      MultipleChoiceQuestion question,
                                      MultipleChoiceQuestionAnswer answer) {
        super(c, question.getQuestion());
        setUpTable(c, question);
        this.answer = answer;
    }

    public void saveAnswer() {
        answer.setAnswer(adapter.getSelectedOptions());
    }

    private void setUpTable(Context c, MultipleChoiceQuestion question) {
        TextView header = new TextView(c);
        if (question.isSingleAnswer()) {
            header.setText(R.string.chooseOne);
        } else {
            header.setText(R.string.chooseMultiple);
        }
        header.setTextSize(HEADER_SIZE);
        ListView table = new ListView(c);
        LayoutParams params = new LayoutParams(LayoutParams.MATCH_PARENT,
                                               LayoutParams.WRAP_CONTENT);
        params.topMargin = 10;
        params.addRule(RelativeLayout.BELOW, R.id.questionNameLabelId);
        params.addRule(RelativeLayout.CENTER_IN_PARENT);
        table.setLayoutParams(params);
        table.addHeaderView(header);
        table.setDivider(getBackground());
        table.setDividerHeight(5);
        addView(table);

        setUpTableRows(c, table, question);
    }

    private void setUpTableRows(Context c, ListView table, MultipleChoiceQuestion question) {
        adapter = new SelectListAdapter<>(question.getOptions(), question.isSingleAnswer(), c);
        table.setAdapter(adapter);
    }
}
