package com.cnk.data.experiment;

import android.support.annotation.NonNull;
import android.util.Log;

import com.cnk.communication.NetworkHandler;
import com.cnk.communication.task.Task;
import com.cnk.data.FileHandler;
import com.cnk.data.experiment.survey.Survey;
import com.cnk.data.raports.Raport;
import com.cnk.data.raports.RaportEvent;
import com.cnk.data.raports.ReadyRaports;
import com.cnk.database.DatabaseHelper;
import com.cnk.database.realm.RaportFileRealm;
import com.cnk.utilities.Consts;
import com.cnk.utilities.Util;

import java.io.File;
import java.io.IOException;
import java.util.List;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class ExperimentData {
    public interface ExperimentUpdateAction {
        void doOnUpdate();
    }

    private class BgRaportSaver implements Runnable {
        private volatile Raport raport;

        public BgRaportSaver(Raport raport) {
            this.raport = raport;
        }

        @Override
        public void run() {
            while (raport.getState() != Raport.State.SENT) {
                raportLock.lock();
                if (!saveRaport()) {
                    raportLock.unlock();
                    Util.waitDelay(15 * Consts.MILLIS_IN_SEC);
                    continue;
                }
                if (raport.getState() == Raport.State.READY_TO_SEND) {
                    raportLock.unlock();
                    break;
                }
                raportLock.unlock();
                Util.waitDelay(15 * Consts.MILLIS_IN_SEC);
            }
            Log.i(LOG_TAG, "Saving raport stopped");
        }

        private boolean saveRaport() {
            String dir = Consts.DATA_PATH + RAPORT_DIRECTORY;
            new File(dir).mkdirs();
            String tmpFile = dir + TMP + raport.getId().toString();
            try {
                FileHandler.getInstance().saveSerializable(raport, tmpFile);
                String realFile = RAPORT_FILE_PREFIX + raport.getId().toString();
                Log.i(LOG_TAG, "Saving raport to file " + realFile);
                FileHandler.getInstance().renameFile(tmpFile, realFile);
            } catch (IOException e) {
                Log.i(LOG_TAG, "Saving raport failed");
                return false;
            }
            return true;
        }
    }

    private static final String LOG_TAG = "ExperimentData";
    private static final String RAPORT_DIRECTORY = "raports/";
    private static final String RAPORT_FILE_PREFIX = "raport";
    private static final String TMP = "TMP";
    private static final long EXPERIMENT_DATA_DOWNLOAD_TIMEOUT = Consts.MILLIS_IN_SEC * 20;
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
    }

    public void downloadExperiment(ExperimentUpdateAction action, Task.TimeoutAction timeoutAction) {
        NetworkHandler.getInstance()
                      .downloadExperimentData(action, timeoutAction, EXPERIMENT_DATA_DOWNLOAD_TIMEOUT);
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

        String path = getCurrentRaportPath();
        new Thread(new BgRaportSaver(currentRaport)).start();

        dbHelper.setRaportFile(newId, path);
    }

    public void addEventToCurrentRaportInBg(RaportEvent event) {
        new Thread(() -> addEventToCurrentRaport(event)).run();
    }

    public void addEventToCurrentRaport(RaportEvent event) {
        raportLock.lock();
        currentRaport.addEvent(event);
        raportLock.unlock();
    }

    public void finishExperiment() {
        markRaportAsReady();
        experiment = null;
    }

    private void markRaportAsReady() {
        raportLock.lock();
        currentRaport.markAsReady();
        ReadyRaports.getInstance().addNewReadyRaport(currentRaport);
        dbHelper.changeRaportState(currentRaport.getId(), RaportFileRealm.READY_TO_SEND);
        currentRaport = null;
        raportLock.unlock();
    }

    private String getCurrentRaportPath() {
        String dir = Consts.DATA_PATH + RAPORT_DIRECTORY;
        new File(dir).mkdirs();
        String realFile = RAPORT_FILE_PREFIX + currentRaport.getId().toString();
        return dir + realFile;
    }
}
