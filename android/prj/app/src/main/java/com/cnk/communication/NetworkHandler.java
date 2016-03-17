package com.cnk.communication;

import android.util.Log;

import com.cnk.communication.task.BackgroundDownloadTask;
import com.cnk.communication.task.ExhibitDownloadTask;
import com.cnk.communication.task.ExperimentDataDownloadTask;
import com.cnk.communication.task.MapDownloadTask;
import com.cnk.communication.task.RaportUploadTask;
import com.cnk.communication.task.Task;
import com.cnk.communication.task.WaitTask;
import com.cnk.data.experiment.ExperimentData;
import com.cnk.data.map.MapData;
import com.cnk.notificators.Notificator;
import com.cnk.utilities.Consts;

import java.util.Observable;
import java.util.Observer;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.LinkedBlockingQueue;

public class NetworkHandler implements Observer {

    private class QueueThread implements Runnable {
        private static final int TRIES = 3;
        private BlockingQueue<Task> queue;

        public QueueThread(BlockingQueue<Task> queue) {
            this.queue = queue;
        }

        public void run() {
            while (true) {
                try {
                    Log.i(LOG_TAG, "Starting another task");
                    queue.take().run(TRIES);
                    Log.i(LOG_TAG, "Task finished");
                } catch (InterruptedException e) {
                    e.printStackTrace();
                    Log.i(LOG_TAG, "Thread woken");
                }
            }
        }
    }
    private static final String LOG_TAG = "NetworkHandler";
    private static final long SECONDS_DELAY = 30;
    private static NetworkHandler instance;
    private BlockingQueue<Task> tasks;
    private BlockingQueue<Task> bgTasks;
    private Notificator mapDownload;
    private Notificator raportUpload;
    private Notificator exhibitsDownload;
    private Notificator bgDownload;
    private Notificator experimentDataDownload;
    private boolean downloadInBg;

    private NetworkHandler() {
        mapDownload = new Notificator(this);
        raportUpload = new Notificator(this);
        exhibitsDownload = new Notificator(this);
        bgDownload = new Notificator(this);
        experimentDataDownload = new Notificator(this);
        downloadInBg = false;
        tasks = new LinkedBlockingQueue<>();
        bgTasks = new LinkedBlockingQueue<>();

        spawnThread(tasks);
        spawnThread(bgTasks);
    }

    public static NetworkHandler getInstance() {
        if (instance == null) {
            instance = new NetworkHandler();
        }
        return instance;
    }

    private void spawnThread(BlockingQueue<Task> tasks) {
        Thread thread = new Thread(new QueueThread(tasks));
        thread.start();
    }

    public synchronized void downloadExperimentData(ExperimentData.ExperimentUpdateAction action) {
        Task task = new ExperimentDataDownloadTask(experimentDataDownload, action);
        tasks.add(task);
    }

    public synchronized void downloadMap(MapData.MapUpdateAction action) {
        Task task = new MapDownloadTask(mapDownload, action);
        tasks.add(task);
    }

    public synchronized void uploadRaports() {
        Task task = new RaportUploadTask(raportUpload);
        tasks.add(task);
    }

    public synchronized void downloadExhibits() {
        Task task = new ExhibitDownloadTask(exhibitsDownload);
        tasks.add(task);
    }

    public synchronized void startBgDownload() {
        if (downloadInBg) {
            return;
        }
        downloadInBg = true;
        addBgDownloadTask();
    }

    public synchronized void stopBgDownload() {
        downloadInBg = false;
    }

    public void update(Observable o, Object arg) {
        if (o == bgDownload && downloadInBg) {
            addBgDownloadTask();
        } else if (arg != null) {
            onFailure((Task) arg);
        } else {
            onSuccess(o);
        }
    }

    private synchronized void addBgDownloadTask() {
        Task task = new BackgroundDownloadTask(bgDownload);
        Task wait = new WaitTask(SECONDS_DELAY * Consts.MILLIS_IN_SEC);
        bgTasks.add(task);
        bgTasks.add(wait);
    }

    private synchronized void onSuccess(Observable o) {
        Log.i(LOG_TAG, "Task finished successfully");
    }

    private synchronized void onFailure(Task task) {
        Log.i(LOG_TAG, "Task failed, retrying");
        tasks.add(task);
    }
}
