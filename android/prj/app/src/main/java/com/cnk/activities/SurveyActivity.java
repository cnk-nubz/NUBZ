package com.cnk.activities;

import android.content.Context;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.inputmethod.InputMethodManager;
import android.widget.RelativeLayout;
import android.widget.TextView;

import com.cnk.R;
import com.cnk.data.DataHandler;
import com.cnk.ui.questions.QuestionView;
import com.cnk.ui.questions.QuestionViewFactory;

import java.util.List;

public class SurveyActivity extends AppCompatActivity {

    private int allQuestionsCount;
    private int currentQuestionNo;
    private TextView counterLabel;
    private RelativeLayout mainView;
    private QuestionView currentQuestionView;
    private List<QuestionView> questionViews;
    private MenuItem nextItem;
    private MenuItem prevItem;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_survey);
        mainView = (RelativeLayout) findViewById(R.id.mainView);
        questionViews = QuestionViewFactory.createQuestionViews(DataHandler.getInstance().getPreSurvey(), this);
        allQuestionsCount = DataHandler.getInstance().getPreSurvey().getRemainingQuestionsCount();
        setUpCounterLabel();
        showView(0);
    }

    @Override
    public boolean onOptionsItemSelected(final MenuItem item) {
        hideKeyboard();
        if (item.getItemId() == R.id.action_prev_question) {
            nextItem.setEnabled(true);
            if (currentQuestionNo - 1 <= 0) {
                prevItem.setEnabled(false);
            }
            showView(currentQuestionNo - 1);
        } else if (item.getItemId() == R.id.action_next_question) {
            prevItem.setEnabled(true);
            if (currentQuestionNo + 1 >= allQuestionsCount - 1) {
                nextItem.setEnabled(false);
            }
            showView(currentQuestionNo + 1);
        }
        return super.onOptionsItemSelected(item);
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        MenuInflater inflater = getMenuInflater();
        inflater.inflate(R.menu.survey_activity_menu, menu);
        prevItem = menu.getItem(0);
        nextItem = menu.getItem(1);
        prevItem.setEnabled(false);
        return true;
    }

    private void setUpCounterLabel() {
        counterLabel = (TextView) mainView.findViewById(R.id.counterLabel);
        updateCounterLabel();
    }

    private void updateCounterLabel() {
        counterLabel.setText(
                Integer.toString(currentQuestionNo + 1) +
                        "/" +
                        Integer.toString(allQuestionsCount)
        );
    }

    private void showView(Integer no) {
        Log.i("SurveyActivity", no.toString());
        currentQuestionNo = no;
        mainView.removeView(currentQuestionView);
        currentQuestionView = questionViews.get(no);
        mainView.addView(currentQuestionView);
        updateCounterLabel();
    }

    public void hideKeyboard() {
        InputMethodManager mgr = (InputMethodManager) getSystemService(Context.INPUT_METHOD_SERVICE);
        mgr.hideSoftInputFromWindow(currentQuestionView.getWindowToken(), 0);
    }
}
