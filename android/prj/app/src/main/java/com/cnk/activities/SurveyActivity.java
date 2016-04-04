package com.cnk.activities;

import android.app.ProgressDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.view.inputmethod.InputMethodManager;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;
import android.widget.TextView;
import android.widget.Toast;

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
import com.cnk.ui.Utils;
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
    private LinearLayout goPrev;
    private LinearLayout goNext;
    private TextView nextText;
    private QuestionView currentQuestionView;
    private List<QuestionView> questionViews;
    private ProgressDialog spinner;
    private Survey.SurveyType type;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_survey);
        mainView = (RelativeLayout) findViewById(R.id.mainView);
        setButtons();
        questionViews = new ArrayList<>();
        type = (Survey.SurveyType) getIntent().getSerializableExtra("type");
        if (type == Survey.SurveyType.BEFORE) {
            Log.i(LOG_TAG, "Survey before");
            startExperimentDataDownload();
        } else {
            Log.i(LOG_TAG, "Survey after");
            init();
        }
    }

    private void startExperimentDataDownload() {
        setSpinner();
        ExperimentData.getInstance()
                      .downloadExperiment(this::experimentDataDownloaded,
                                          this::experimetDataDownloadingTimeout);
    }

    private void setButtons() {
        goPrev = (LinearLayout) findViewById(R.id.goPrev);
        goPrev.setOnClickListener(this::defaultPrevAction);
        goNext = (LinearLayout) findViewById(R.id.goNext);
        nextText = (TextView) findViewById(R.id.nextText);
    }

    private void defaultNextAction(View view) {
        currentQuestionView.saveAnswer();
        if (!currentQuestionView.canContinue()) {
            Toast.makeText(this, R.string.notFilled, Toast.LENGTH_SHORT).show();
            return;
        }
        hideKeyboard();
        currentQuestionNo++;
        showView(currentQuestionNo);
    }

    private void defaultPrevAction(View view) {
        currentQuestionView.saveAnswer();
        hideKeyboard();
        currentQuestionNo--;
        showView(currentQuestionNo);
    }

    private void refreshButtons() {
        if (currentQuestionNo == 0) {
            goPrev.setVisibility(View.INVISIBLE);
        } else {
            goPrev.setVisibility(View.VISIBLE);
        }

        if (currentQuestionNo == allQuestionsCount - 1) {
            nextText.setText(R.string.finish);
            goNext.setOnClickListener(this::finishButtonAction);
        } else {
            nextText.setText(R.string.next);
            goNext.setOnClickListener(this::defaultNextAction);
        }
    }

    private void finishButtonAction(View view) {
        currentQuestionView.saveAnswer();
        if (!currentQuestionView.canContinue()) {
            Toast.makeText(this, R.string.notFilled, Toast.LENGTH_SHORT).show();
            return;
        }
        hideKeyboard();
        DialogInterface.OnClickListener positiveAction = (dialog, which) -> changeToNextActivity();
        Utils.showDialog(SurveyActivity.this,
                         R.string.confirmation,
                         R.string.confirm,
                         R.string.cancel,
                         positiveAction);
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
    protected void onDestroy() {
        super.onDestroy();
        goNext.setOnClickListener(null);
        goPrev.setOnClickListener(null);
    }

    private void setSpinner() {
        spinner = new ProgressDialog(this);
        spinner.setTitle(getString(R.string.loading));
        spinner.setMessage(getString(R.string.waitingForExperimentData));
        spinner.setCancelable(false);
        spinner.show();
    }

    private void experimentDataDownloaded() {
        init();
        if (spinner != null) {
            spinner.dismiss();
        }
    }

    private void experimetDataDownloadingTimeout() {
        runOnUiThread(() -> {
            spinner.dismiss();
            DialogInterface.OnClickListener
                    positiveAction =
                    (dialog, which) -> startExperimentDataDownload();
            DialogInterface.OnClickListener
                    negativeAction =
                    (dialog, which) -> SurveyActivity.this.finish();
            Utils.showDialog(SurveyActivity.this,
                             R.string.experimentDataDownloadTimeout,
                             R.string.confirm,
                             R.string.cancel,
                             positiveAction,
                             negativeAction);
        });
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

    private void showView(int no) {
        currentQuestionNo = no;
        mainView.removeView(currentQuestionView);
        currentQuestionView = questionViews.get(no);
        mainView.addView(currentQuestionView);
        refreshButtons();
        updateCounterLabel();
    }


    @Override
    public void onBackPressed() {
        // overriden to stop back button from working
    }

    private void updateCounterLabel() {
        counterLabel.setText((currentQuestionNo + 1) + "/" + (allQuestionsCount));
    }

    private void hideKeyboard() {
        InputMethodManager
                mgr =
                (InputMethodManager) getSystemService(Context.INPUT_METHOD_SERVICE);
        mgr.hideSoftInputFromWindow(currentQuestionView.getWindowToken(), 0);
    }
}
