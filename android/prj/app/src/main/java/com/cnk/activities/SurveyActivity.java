package com.cnk.activities;

import android.app.ProgressDialog;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AlertDialog;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.inputmethod.InputMethodManager;
import android.widget.RelativeLayout;
import android.widget.TextView;

import com.cnk.R;
import com.cnk.communication.NetworkHandler;
import com.cnk.data.experiment.ExperimentData;
import com.cnk.data.experiment.survey.Survey;
import com.cnk.data.experiment.survey.answers.MultipleChoiceQuestionAnswer;
import com.cnk.data.experiment.survey.answers.SimpleQuestionAnswer;
import com.cnk.data.experiment.survey.answers.SortQuestionAnswer;
import com.cnk.data.experiment.survey.answers.SurveyAnswers;
import com.cnk.data.experiment.survey.questions.MultipleChoiceQuestion;
import com.cnk.data.experiment.survey.questions.SortQuestion;
import com.cnk.notificators.Observer;
import com.cnk.ui.questions.QuestionView;
import com.cnk.ui.questions.QuestionViewFactory;

import java.util.ArrayList;
import java.util.List;

public class SurveyActivity extends AppCompatActivity implements Observer {

    private static final String LOG_TAG = "SurveyActivity";

    private int allQuestionsCount;
    private int currentQuestionNo;
    private TextView counterLabel;
    private RelativeLayout mainView;
    private QuestionView currentQuestionView;
    private List<QuestionView> questionViews;
    private MenuItem nextItem;
    private MenuItem prevItem;
    private ProgressDialog spinner;
    private Survey.SurveyType type;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_survey);
        mainView = (RelativeLayout) findViewById(R.id.mainView);
        questionViews = new ArrayList<>();
        type = (Survey.SurveyType) getIntent().getSerializableExtra("type");
        if (type == Survey.SurveyType.BEFORE) {
            Log.i(LOG_TAG, "Survey before");
            setSpinner();
            ExperimentData.getInstance().downloadExperiment(this::experimentDataDownloaded);
        } else {
            Log.i(LOG_TAG, "Survey after");
            init();
        }
    }

    private void setSpinner() {
        spinner = new ProgressDialog(this);
        spinner.setTitle("Ładowanie");
        spinner.setMessage("Oczekiwanie na pobranie danych");
        spinner.setCancelable(false);
        spinner.show();
    }

    private void experimentDataDownloaded() {
        init();
        if (spinner != null) {
            spinner.dismiss();
        }
    }

    private void init() {
        if (type == Survey.SurveyType.BEFORE) {
            ExperimentData.getInstance().startNewRaport();
        }
        runOnUiThread(() -> {
            initViews();
            if (allQuestionsCount == 0) {
                changeToNextActivity();
                return;
            }
            setUpCounterLabel();
            showView(0);
        });
    }

    private void initViews() {
        Survey currentSurvey = ExperimentData.getInstance().getSurvey(type);
        SurveyAnswers answers = currentSurvey.getSurveyAnswers();
        allQuestionsCount = currentSurvey.getRemainingQuestionsCount();
        while (currentSurvey.getRemainingQuestionsCount() > 0) {
            Survey.QuestionType questionType = currentSurvey.popNextQuestionType();
            switch (questionType) {
                case SIMPLE:
                    SimpleQuestionAnswer simpleAnswer = new SimpleQuestionAnswer();
                    answers.addSimpleAnswer(simpleAnswer);
                    questionViews.add(QuestionViewFactory.createQuestionView(currentSurvey.popNextSimpleQuestion(),
                                                                             this,
                                                                             simpleAnswer));
                    break;
                case MULTIPLE_CHOICE:
                    MultipleChoiceQuestionAnswer
                            multipleChoiceAnswer =
                            new MultipleChoiceQuestionAnswer();
                    answers.addMultipleChoiceAnswer(multipleChoiceAnswer);
                    MultipleChoiceQuestion
                            nextMultipleChoiceQuestion =
                            currentSurvey.popNextMultipleChoiceQuestion();
                    questionViews.add(QuestionViewFactory.createQuestionView(nextMultipleChoiceQuestion,
                                                                             this,
                                                                             multipleChoiceAnswer));
                    break;
                case SORT:
                    SortQuestionAnswer sortAnswer = new SortQuestionAnswer();
                    answers.addSortQuestionAnswer(sortAnswer);
                    SortQuestion nextSortQuestion = currentSurvey.popNextSortQuestions();
                    questionViews.add(QuestionViewFactory.createQuestionView(nextSortQuestion,
                                                                             this,
                                                                             sortAnswer));
                    break;
            }
        }
    }

    private void setUpCounterLabel() {
        counterLabel = (TextView) mainView.findViewById(R.id.counterLabel);
        updateCounterLabel();
    }

    private void showView(Integer no) {
        currentQuestionNo = no;
        mainView.removeView(currentQuestionView);
        currentQuestionView = questionViews.get(no);
        mainView.addView(currentQuestionView);
        updateCounterLabel();
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

    @Override
    public boolean onOptionsItemSelected(final MenuItem item) {
        hideKeyboard();
        currentQuestionView.saveAnswer();
        if (item.getItemId() == R.id.action_prev_question) {
            nextItem.setTitle(R.string.next);
            if (currentQuestionNo - 1 <= 0) {
                prevItem.setEnabled(false);
            }
            showView(currentQuestionNo - 1);
        } else if (item.getItemId() == R.id.action_next_question) {
            prevItem.setEnabled(true);
            if (currentQuestionNo >= allQuestionsCount - 1) {
                showDialog();
            } else {
                if (currentQuestionNo == allQuestionsCount - 2) {
                    nextItem.setTitle(R.string.finish);
                }
                showView(currentQuestionNo + 1);
            }

        }
        return super.onOptionsItemSelected(item);
    }

    private void showDialog() {
        AlertDialog.Builder alert = new AlertDialog.Builder(this);
        alert.setMessage(R.string.confirmation);
        alert.setPositiveButton("Tak", (dialog, which) -> {
            changeToNextActivity();
        });
        alert.setNegativeButton("Nie", (dialog, which) -> {
        });
        alert.setCancelable(false);
        alert.show();
    }

    private void changeToNextActivity() {
        Class next = (Class) getIntent().getSerializableExtra("nextActivity");
        if (next == null) {
            finish();
            ExperimentData.getInstance().finishExperiment();
            NetworkHandler.getInstance().uploadRaports();
        } else {
            Intent i = new Intent(getApplicationContext(), next);
            finish();
            startActivity(i);
        }
    }

    @Override
    public void onBackPressed() {
        // overriden to stop back button from working
    }

    private void updateCounterLabel() {
        counterLabel.setText(Integer.toString(currentQuestionNo + 1) +
                             "/" +
                             Integer.toString(allQuestionsCount));
    }

    private void hideKeyboard() {
        InputMethodManager
                mgr =
                (InputMethodManager) getSystemService(Context.INPUT_METHOD_SERVICE);
        mgr.hideSoftInputFromWindow(currentQuestionView.getWindowToken(), 0);
    }
}
