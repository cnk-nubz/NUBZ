package com.cnk.communication.task;

import android.util.Log;

import com.cnk.communication.thrift.MultipleChoiceQuestionAnswer;
import com.cnk.communication.thrift.RawReport;
import com.cnk.communication.thrift.RawReportEvent;
import com.cnk.communication.thrift.Server;
import com.cnk.communication.thrift.SimpleQuestionAnswer;
import com.cnk.communication.thrift.SortQuestionAnswer;
import com.cnk.communication.thrift.SurveyAnswers;
import com.cnk.data.DataHandler;
import com.cnk.data.raport.Raport;
import com.cnk.data.raport.RaportEvent;
import com.cnk.notificators.Notificator;

import org.apache.thrift.TException;

import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;

public class RaportUploadTask extends ServerTask {

    private static final String LOG_TAG = "RaportUploadTask";

    public RaportUploadTask(Notificator notificator) {
        super(notificator);
    }

    @Override
    protected void performInSession(Server.Client client) throws TException {
        Map<Raport, Integer> toSend = DataHandler.getInstance().getAllReadyRaports();
        for (Map.Entry<Raport, Integer> entry : toSend.entrySet()) {
            Raport raport = entry.getKey();
            Integer serverId = entry.getValue();
            if (serverId == null) {
                serverId = client.getIdForNewReport();
                DataHandler.getInstance().setServerId(raport, serverId);
            }
            Log.i(LOG_TAG, "Sending raport with id " + raport.getId().toString());
            RawReport thriftRaport = translateToThrift(raport, serverId);
            client.saveReport(thriftRaport);
            Log.i(LOG_TAG, "Raport sent");
            DataHandler.getInstance().markRaportAsSent(raport);
            toSend.remove(entry.getKey());
        }
    }

    private RawReport translateToThrift(Raport raport, Integer serverId) {
        RawReport rawRaport = new RawReport();
        rawRaport.setAnswersBefore(translateSurvey(raport.getPreSurveyAnswers()));
        rawRaport.setHistory(translateActions(raport.getHistory()));
        rawRaport.setAnswersAfter(translateSurvey(raport.getPostSurveyAnswers()));
        rawRaport.setReportId(serverId);
        rawRaport.setExperimentId(raport.getExperimentId());
        return rawRaport;
    }

    private List<RawReportEvent> translateActions(List<RaportEvent> events) {
        List<RawReportEvent> thriftEvents = new LinkedList<>();
        for (RaportEvent event : events) {
            RawReportEvent rawEvent = new RawReportEvent();
            rawEvent.setActions(event.getActions());
            rawEvent.setDurationInSecs(event.getDurationSeconds());
            if (event.getExhibitId() != null) {
                rawEvent.setExhibitId(event.getExhibitId());
            }
            thriftEvents.add(rawEvent);
        }
        return thriftEvents;
    }

    private SurveyAnswers translateSurvey(com.cnk.data.experiment.answers.SurveyAnswers answers) {
        SurveyAnswers thriftAnswers = new SurveyAnswers();
        thriftAnswers.setSimpleQuestionsAnswers(translateSimpleAnswers(answers.getSimpleAnswers()));
        thriftAnswers.setMultipleChoiceQuestionsAnswers(translateMultiQuestions(answers.getMultipleChoiceAnswers()));
        thriftAnswers.setSortQuestionsAnswers(trasnlateSortQuestions(answers.getSortQuestionAnswers()));
        return thriftAnswers;
    }

    private List<SimpleQuestionAnswer> translateSimpleAnswers(List<com.cnk.data.experiment.answers.SimpleQuestionAnswer> answers) {
        Log.i(LOG_TAG, "-------SIMPLE QUESTIONS-------");
        List<SimpleQuestionAnswer> thriftAnswers = new ArrayList<>();
        for (com.cnk.data.experiment.answers.SimpleQuestionAnswer answer : answers) {
            Log.i(LOG_TAG, "SIMPLE QUESTION");
            SimpleQuestionAnswer thriftAnswer = new SimpleQuestionAnswer();
            Log.i(LOG_TAG, answer.getAnswer());
            thriftAnswer.setAnswer(answer.getAnswer());
            thriftAnswers.add(thriftAnswer);
            Log.i(LOG_TAG, " ");
        }
        return thriftAnswers;
    }

    private List<MultipleChoiceQuestionAnswer> translateMultiQuestions(List<com.cnk.data.experiment.answers.MultipleChoiceQuestionAnswer> answers) {
        Log.i(LOG_TAG, "-------MULTIPLE CHOICE QUESTIONS-------");
        List<MultipleChoiceQuestionAnswer> thriftAnswers = new ArrayList<>();
        for (com.cnk.data.experiment.answers.MultipleChoiceQuestionAnswer answer : answers) {
            Log.i(LOG_TAG, "MULTIPLE CHOICE QUESTION");
            MultipleChoiceQuestionAnswer thriftAnswer = new MultipleChoiceQuestionAnswer();
            List<Integer> thriftOptions = new ArrayList<>();
            if (answer.getAnswer() == null) {
                thriftOptions = null;
            } else {
                for (Integer option : answer.getAnswer()) {
                    Log.i(LOG_TAG, option.toString());
                    thriftOptions.add(option);
                }
            }
            thriftAnswer.setChoosenOptions(thriftOptions);
            thriftAnswers.add(thriftAnswer);
            Log.i(LOG_TAG, " ");
        }
        return thriftAnswers;
    }

    private List<SortQuestionAnswer> trasnlateSortQuestions(List<com.cnk.data.experiment.answers.SortQuestionAnswer> answers) {
        Log.i(LOG_TAG, "-------SORT QUESTIONS-------");
        List<SortQuestionAnswer> thriftAnswers = new ArrayList<>();
        for (com.cnk.data.experiment.answers.SortQuestionAnswer answer : answers) {
            Log.i(LOG_TAG, "SORT QUESTION");
            SortQuestionAnswer thriftAnswer = new SortQuestionAnswer();
            List<Integer> thriftOptions = new ArrayList<>();
            if (answer.getAnswer() == null) {
                thriftOptions = null;
            } else {
                for (Integer option : answer.getAnswer()) {
                    Log.i(LOG_TAG, option.toString());
                    thriftOptions.add(option);
                }
            }
            thriftAnswer.setChoosenOrder(thriftOptions);
            thriftAnswers.add(thriftAnswer);
            Log.i(LOG_TAG, " ");
        }
        return thriftAnswers;
    }

}
