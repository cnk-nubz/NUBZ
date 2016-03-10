package com.cnk.ui.questions;

import android.content.Context;
import android.widget.ListView;
import android.widget.RelativeLayout;
import android.widget.TextView;

import com.cnk.R;
import com.cnk.data.experiment.survey.answers.SortQuestionAnswer;
import com.cnk.data.experiment.survey.questions.SortQuestion;
import com.cnk.data.experiment.survey.questions.SortQuestionOption;
import com.cnk.ui.adapters.SortListAdapter;

public class SortQuestionView extends QuestionView {

    private static final float HEADER_SIZE = 25.0f;
    private SortListAdapter<SortQuestionOption> adapter;
    private SortQuestionAnswer answer;

    public SortQuestionView(Context c, SortQuestion question, SortQuestionAnswer answer) {
        super(c, question.getQuestion());
        this.answer = answer;
        setUpTable(c, question);
    }

    public void saveAnswer() {
        answer.setAnswer(adapter.getOrder());
    }

    private void setUpTable(Context c, SortQuestion question) {
        TextView header = new TextView(c);
        header.setText(R.string.sort);
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

    private void setUpTableRows(Context c, ListView table, SortQuestion question) {
        adapter = new SortListAdapter<>(question.getOptions(), c);
        table.setAdapter(adapter);
    }
}
