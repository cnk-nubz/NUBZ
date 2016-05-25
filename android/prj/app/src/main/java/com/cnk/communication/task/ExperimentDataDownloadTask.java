package com.cnk.communication.task;

import android.util.Log;

import com.cnk.communication.NetworkHandler;
import com.cnk.communication.thrift.CurrentExperimentResponse;
import com.cnk.communication.thrift.QuestionType;
import com.cnk.communication.thrift.QuestionsList;
import com.cnk.communication.thrift.Server;
import com.cnk.data.experiment.Action;
import com.cnk.data.experiment.Experiment;
import com.cnk.data.experiment.ExperimentData;
import com.cnk.data.experiment.survey.Survey;
import com.cnk.data.experiment.survey.questions.MultipleChoiceQuestion;
import com.cnk.data.experiment.survey.questions.MultipleChoiceQuestionOption;
import com.cnk.data.experiment.survey.questions.SimpleQuestion;
import com.cnk.data.experiment.survey.questions.SortQuestion;
import com.cnk.data.experiment.survey.questions.SortQuestionOption;
import com.cnk.exceptions.NoExperimentException;

import org.apache.thrift.TException;

import java.io.IOException;
import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;
import java.util.Queue;

public class ExperimentDataDownloadTask extends ServerTask {

    private static final String LOG_TAG = "ExperimentDownloadTask";

    public ExperimentDataDownloadTask(NetworkHandler.SuccessAction success,
                                      NetworkHandler.FailureAction failure) {
        super(success, failure);
    }

    @Override
    protected void performInSession(Server.Client client) throws TException, IOException, NoExperimentException {
        Log.i(LOG_TAG, "Downloading experiment");
        CurrentExperimentResponse thriftData = client.getCurrentExperiment();
        if (thriftData.getExperiment() == null) {
            Log.i(LOG_TAG, "No active experiment");
            throw new NoExperimentException();
        }
        Log.i(LOG_TAG, "Downloaded experiment");
        updateDataHandler(thriftData);
    }

    private void updateDataHandler(CurrentExperimentResponse thriftData) {
        Experiment experiment = translateDataFromThrift(thriftData);
        ExperimentData.getInstance().setNewExperimentData(experiment);
    }

    private Experiment translateDataFromThrift(CurrentExperimentResponse thriftData) {
        Integer id = thriftData.getExperiment().getExperimentId();
        String name = thriftData.getExperiment().getName();
        List<Action> exhibitActions =
                translateActionsFromThrift(thriftData.getExperiment().getExhibitActions());
        List<Action> breakActions =
                translateActionsFromThrift(thriftData.getExperiment().getBreakActions());
        Survey preSurvey = translateSurveyFromThrift(thriftData.getExperiment().getSurveyBefore());
        Survey postSurvey = translateSurveyFromThrift(thriftData.getExperiment().getSurveyAfter());
        return new Experiment(id, name, exhibitActions, breakActions, preSurvey, postSurvey);
    }

    private List<Action> translateActionsFromThrift(List<com.cnk.communication.thrift.Action> thriftActions) {
        List<Action> actions = new ArrayList<>();
        for (com.cnk.communication.thrift.Action a : thriftActions) {
            actions.add(new Action(a.getActionId(), a.getText()));
        }
        return actions;
    }

    private Survey translateSurveyFromThrift(QuestionsList survey) {
        Queue<Survey.QuestionType> types = questionOrderFromThrift(survey.getQuestionsOrder());
        Queue<SimpleQuestion> simpleQs = simpleQuestionsFromThrift(survey.getSimpleQuestions());
        Queue<MultipleChoiceQuestion> multiQs =
                multiQuestionsFromThrift(survey.getMultipleChoiceQuestions());
        Queue<SortQuestion> sortQs = sortQuestionsFromThrift(survey.getSortQuestions());

        return new Survey(types, simpleQs, multiQs, sortQs);
    }

    private Queue<Survey.QuestionType> questionOrderFromThrift(List<QuestionType> thrift) {
        Queue<Survey.QuestionType> types = new LinkedList<>();
        for (QuestionType type : thrift) {
            switch (type) {
                case SIMPLE:
                    types.add(Survey.QuestionType.SIMPLE);
                    break;
                case MULTIPLE_CHOICE:
                    types.add(Survey.QuestionType.MULTIPLE_CHOICE);
                    break;
                case SORT:
                    types.add(Survey.QuestionType.SORT);
                    break;
            }
        }
        return types;
    }

    private Queue<SimpleQuestion> simpleQuestionsFromThrift(List<com.cnk.communication.thrift.SimpleQuestion> thrift) {
        Queue<SimpleQuestion> questions = new LinkedList<>();
        for (com.cnk.communication.thrift.SimpleQuestion question : thrift) {
            SimpleQuestion.AnswerType type = null;
            switch (question.getAnswerType()) {
                case TEXT:
                    type = SimpleQuestion.AnswerType.TEXT;
                    break;
                case NUMBER:
                    type = SimpleQuestion.AnswerType.NUMBER;
                    break;
            }
            questions.add(new SimpleQuestion(question.questionId, question.getQuestion(), type));
        }
        return questions;
    }

    private Queue<MultipleChoiceQuestion> multiQuestionsFromThrift(List<com.cnk.communication.thrift.MultipleChoiceQuestion> thrift) {
        Queue<MultipleChoiceQuestion> questions = new LinkedList<>();
        for (com.cnk.communication.thrift.MultipleChoiceQuestion quesiton : thrift) {
            List<MultipleChoiceQuestionOption> options = new ArrayList<>();
            for (com.cnk.communication.thrift.MultipleChoiceQuestionOption option : quesiton.getOptions()) {
                options.add(new MultipleChoiceQuestionOption(option.optionId, option.getText()));
            }
            questions.add(new MultipleChoiceQuestion(quesiton.questionId,
                                                     quesiton.getQuestion(),
                                                     quesiton.isSingleAnswer(),
                                                     options));
        }
        return questions;
    }

    private Queue<SortQuestion> sortQuestionsFromThrift(List<com.cnk.communication.thrift.SortQuestion> thrift) {
        Queue<SortQuestion> questions = new LinkedList<>();
        for (com.cnk.communication.thrift.SortQuestion quesiton : thrift) {
            List<SortQuestionOption> options = new ArrayList<>();
            for (com.cnk.communication.thrift.SortQuestionOption option : quesiton.getOptions()) {
                options.add(new SortQuestionOption(option.optionId, option.getText()));
            }
            questions.add(new SortQuestion(quesiton.questionId, quesiton.getQuestion(), options));
        }
        return questions;
    }
}
