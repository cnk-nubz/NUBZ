package com.cnk.activities;

import android.app.ProgressDialog;
import android.content.Context;
import android.content.DialogInterface;
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
import com.cnk.data.DataHandler;
import com.cnk.data.experiment.Survey;
import com.cnk.data.experiment.answers.MultipleChoiceQuestionAnswer;
import com.cnk.data.experiment.answers.SimpleQuestionAnswer;
import com.cnk.data.experiment.answers.SortQuestionAnswer;
import com.cnk.data.experiment.answers.SurveyAnswers;
import com.cnk.data.experiment.questions.MultipleChoiceQuestion;
import com.cnk.data.experiment.questions.SortQuestion;
import com.cnk.ui.questions.QuestionView;
import com.cnk.ui.questions.QuestionViewFactory;

import java.util.ArrayList;
import java.util.List;
import java.util.Observable;
import java.util.Observer;

public class SurveyActivity extends AppCompatActivity implements Observer {

    private static final String LOG_TAG = "SurveyActivity";

    private int allQuestionsCount;
    private int currentQuestionNo;
    private TextView counterLabel;
    private RelativeLayout mainView;
    private QuestionView currentQuestionView;
    private List<QuestionView> questionViews;
    private SurveyAnswers answers;
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
        DataHandler.getInstance().addObserver(this);
        type = (Survey.SurveyType) getIntent().getSerializableExtra("type");
        if (type == Survey.SurveyType.BEFORE) {
            Log.i(LOG_TAG, "Survey before");
            setSpinner();
            NetworkHandler.getInstance().downloadExperimentData();
        } else {
            Log.i(LOG_TAG, "Survey after");
            init();
        }

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

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        MenuInflater inflater = getMenuInflater();
        inflater.inflate(R.menu.survey_activity_menu, menu);
        prevItem = menu.getItem(0);
        nextItem = menu.getItem(1);
        prevItem.setEnabled(false);
        return true;
    }

    private void init() {
        if (type == Survey.SurveyType.BEFORE) {
            DataHandler.getInstance().startNewRaport();
        }
        initViews();
        setUpCounterLabel();
        showView(0);
    }

    private void setSpinner() {
        spinner = new ProgressDialog(this);
        spinner.setTitle("Ładowanie");
        spinner.setMessage("Oczekiwanie na pobranie danych");
        spinner.setCancelable(false);
        spinner.show();
    }

    private void setUpCounterLabel() {
        counterLabel = (TextView) mainView.findViewById(R.id.counterLabel);
        updateCounterLabel();
    }

    private void updateCounterLabel() {
        counterLabel.setText(Integer.toString(currentQuestionNo + 1) +
                             "/" +
                             Integer.toString(allQuestionsCount));
    }

    private void showView(Integer no) {
        currentQuestionNo = no;
        mainView.removeView(currentQuestionView);
        currentQuestionView = questionViews.get(no);
        mainView.addView(currentQuestionView);
        updateCounterLabel();
    }

    private void initViews() {
        Survey currentSurvey = DataHandler.getInstance().getSurvey(type);
        answers = currentSurvey.getSurveyAnswers();
        allQuestionsCount = currentSurvey.getRemainingQuestionsCount();
        while (currentSurvey.getRemainingQuestionsCount() > 0) {
            Survey.QuestionType type = currentSurvey.popNextQuestionType();
            switch (type) {
                case SIMPLE:
                    SimpleQuestionAnswer simpleAnswer = new SimpleQuestionAnswer();
                    answers.addSimpleAnswer(simpleAnswer);
                    questionViews.add(QuestionViewFactory.createQuestionView(currentSurvey.popNextSimpleQuestion(),
                                                                             this,
                                                                             simpleAnswer));
                    break;
                case MULTIPLE_CHOICE:
                    MultipleChoiceQuestionAnswer multipleChoiceAnswer = new MultipleChoiceQuestionAnswer();
                    answers.addMultipleChoiceAnswer(multipleChoiceAnswer);
                    MultipleChoiceQuestion nextMultipleChoiceQuestion = currentSurvey.popNextMultipleChoiceQuestion();
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

    private void hideKeyboard() {
        InputMethodManager mgr = (InputMethodManager) getSystemService(Context.INPUT_METHOD_SERVICE);
        mgr.hideSoftInputFromWindow(currentQuestionView.getWindowToken(), 0);
    }

    // Data updating:
    @Override
    public void update(Observable observable, Object o) {
        DataHandler.Item notification = (DataHandler.Item) o;
        if (notification.equals(DataHandler.Item.EXPERIMENT_DATA)) {
            DataHandler.getInstance().deleteObserver(this);
            experimentDataDownloaded();
        }
    }

    private void experimentDataDownloaded() {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                init();
            }
        });
        spinner.dismiss();
    }

    private void showDialog() {
        AlertDialog.Builder alert = new AlertDialog.Builder(this);
        alert.setMessage(R.string.confirmation);
        alert.setPositiveButton("Tak", new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                Class next = (Class) getIntent().getSerializableExtra("nextActivity");
                if (next == null) {
                    finish();
                    DataHandler.getInstance().markRaportAsReady();
                    NetworkHandler.getInstance().uploadRaport();
                } else {
                    Intent i = new Intent(getApplicationContext(), next);
                    finish();
                    startActivity(i);
                }
            }
        });
        alert.setNegativeButton("Nie", new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                // does nothing
            }
        });
        alert.setCancelable(false);
        alert.show();
    }
}
