package com.cnk.data.experiment;

import android.support.annotation.NonNull;
import android.util.Log;

import com.cnk.data.FileHandler;
import com.cnk.data.experiment.raport.Raport;
import com.cnk.data.experiment.raport.RaportEvent;
import com.cnk.data.experiment.survey.Survey;
import com.cnk.data.raports.ReadyRaports;
import com.cnk.database.DatabaseHelper;
import com.cnk.database.realm.RaportFileRealm;
import com.cnk.notificators.ExperimentObservable;
import com.cnk.utilities.Consts;

import java.io.File;
import java.io.IOException;
import java.util.List;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class ExperimentData extends ExperimentObservable {
    private static final String LOG_TAG = "ExperimentData";
    private static final String RAPORT_DIRECTORY = "raports/";
    private static final String RAPORT_FILE_PREFIX = "raport";
    private static final String TMP = "TMP";
    private static ExperimentData instance;
    private Lock raportLock;
    private DatabaseHelper dbHelper;
    private Raport currentRaport;
    private Experiment experiment;

    private ExperimentData() {
        raportLock = new ReentrantLock(true);
    }

    public static ExperimentData getInstance() {
        if (instance == null) {
            instance = new ExperimentData();
        }
        return instance;
    }

    public void setDbHelper(DatabaseHelper dbHelper) {
        this.dbHelper = dbHelper;
    }

    public void setNewExperimentData(Experiment newData) {
        experiment = newData;
        notifyObservers();
    }

    public Survey getSurvey(@NonNull Survey.SurveyType type) {
        return experiment.getSurvey(type);
    }

    public List<Action> getAllExhibitActions() {
        return experiment.getExhibitActions();
    }

    public List<Action> getAllBreakActions() {
        return experiment.getBreakActions();
    }

    // only creates new database entry and file for new raport which is not used anywhere else
    public void startNewRaport() {
        Integer newId = dbHelper.getNextRaportId();
        currentRaport =
                new Raport(newId,
                           experiment.getId(),
                           experiment.getSurvey(Survey.SurveyType.BEFORE).getSurveyAnswers(),
                           experiment.getSurvey(Survey.SurveyType.AFTER).getSurveyAnswers());
        String path;
        path = saveCurrentRaport();

        dbHelper.setRaportFile(newId, path);
    }

    public void addEventToCurrentRaport(RaportEvent event) {
        raportLock.lock();
        currentRaport.addEvent(event);
        saveCurrentRaport();
        raportLock.unlock();
    }

    // after raport is marked as ready it is only used in one thread so no synchronization is needed
    public void markRaportAsReady() {
        raportLock.lock();
        saveCurrentRaport();
        ReadyRaports.getInstance().addNewReadyRaport(currentRaport);
        dbHelper.changeRaportState(currentRaport.getId(), RaportFileRealm.READY_TO_SEND);
        currentRaport = null;
        raportLock.unlock();
    }

    public void finishExperiment() {
        experiment = null;
    }

    public String saveCurrentRaport() {
        Log.i(LOG_TAG, currentRaport.toString());
        String dir = Consts.DATA_PATH + RAPORT_DIRECTORY;
        new File(dir).mkdirs();
        String tmpFile = dir + TMP;
        String realFile = "";
        try {
            FileHandler.getInstance().saveSerializable(currentRaport, tmpFile);
            realFile = RAPORT_FILE_PREFIX + currentRaport.getId().toString();
            FileHandler.getInstance().renameFile(tmpFile, realFile);
        } catch (IOException e) {
            Log.i(LOG_TAG, "Saving raport failed");
            saveCurrentRaport();
        }
        return dir + realFile;
    }
}
