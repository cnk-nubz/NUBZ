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
        LayoutParams
                headerParams =
                new LayoutParams(LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT);
        headerParams.addRule(RelativeLayout.BELOW, R.id.questionNameLabelId);
        headerParams.setMargins(50, 20, 50, 0);
        header.setLayoutParams(headerParams);
        header.setId(R.id.questionListViewHeaderId);
        header.setTextSize(HEADER_SIZE);
        addView(header);

        ListView table = new ListView(c);
        LayoutParams
                params =
                new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT);
        params.setMargins(50, 10, 50, 30);
        params.addRule(RelativeLayout.BELOW, header.getId());
        table.setLayoutParams(params);
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
