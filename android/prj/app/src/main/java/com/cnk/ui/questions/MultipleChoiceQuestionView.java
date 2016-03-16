package com.cnk.ui.questions;

import android.content.Context;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ListView;
import android.widget.RelativeLayout;
import android.widget.TextView;

import com.cnk.R;
import com.cnk.data.experiment.survey.answers.MultipleChoiceQuestionAnswer;
import com.cnk.data.experiment.survey.questions.MultipleChoiceQuestion;
import com.cnk.data.experiment.survey.questions.MultipleChoiceQuestionOption;
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
                new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.WRAP_CONTENT);
        params.setMargins(50, 10, 50, 30);
        params.addRule(RelativeLayout.BELOW, header.getId());
        params.addRule(RelativeLayout.CENTER_IN_PARENT);
        table.setLayoutParams(params);
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
