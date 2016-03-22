package com.cnk.communication;

import android.util.Log;

import com.cnk.communication.task.ExhibitDownloadTask;
import com.cnk.communication.task.ExperimentDataDownloadTask;
import com.cnk.communication.task.MapDownloadTask;
import com.cnk.communication.task.RaportUploadTask;
import com.cnk.communication.task.Task;
import com.cnk.communication.task.WaitTask;
import com.cnk.data.experiment.ExperimentData;
import com.cnk.data.map.MapData;

import java.util.concurrent.BlockingQueue;
import java.util.concurrent.LinkedBlockingQueue;

public class NetworkHandler {

    public interface FinishAction {
        void perform(Task sender);
    }

    private static final String LOG_TAG = "NetworkHandler";
    private static final long SECONDS_DELAY = 30;
    private static NetworkHandler instance;

    private BlockingQueue<Task> tasks;
    private BlockingQueue<Task> bgTasks;

    private boolean downloadInBg;

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

    private NetworkHandler() {
        downloadInBg = false;
        tasks = new LinkedBlockingQueue<>();
        bgTasks = new LinkedBlockingQueue<>();

        spawnThread(tasks);
        spawnThread(bgTasks);
    }

    private void spawnThread(BlockingQueue<Task> tasks) {
        Thread thread = new Thread(new QueueThread(tasks));
        thread.start();
    }

    public static NetworkHandler getInstance() {
        if (instance == null) {
            instance = new NetworkHandler();
        }
        return instance;
    }

    public synchronized void downloadExperimentData(ExperimentData.ExperimentUpdateAction action) {
        Task task = new ExperimentDataDownloadTask(this::onFailure, this::onSuccess, action);
        tasks.add(task);
    }

    public synchronized void downloadMap(MapData.MapUpdateAction action) {
        Task task = new MapDownloadTask(this::onFailure, this::onSuccess, action);
        tasks.add(task);
    }

    public synchronized void uploadRaports() {
        Task task = new RaportUploadTask(this::onFailure, this::onSuccess);
        tasks.add(task);
    }

    public synchronized void startBgDownload() {
        if (downloadInBg) {
            return;
        }
        downloadInBg = true;
        Task task = new ExhibitDownloadTask(this::onBgFailure, this::onBgSuccess);
        tasks.add(task);
    }

    public synchronized void stopBgDownload() {
        downloadInBg = false;
    }

    private synchronized void onBgSuccess(Task t) {
        Log.i(LOG_TAG, "Bg task finished successfully");
        if (downloadInBg) {
            bgTasks.add(new WaitTask(SECONDS_DELAY));
            bgTasks.add(t);
        }
    }

    private synchronized void onBgFailure(Task t) {
        Log.i(LOG_TAG, "Bg task failed, retrying");
        if (downloadInBg) {
            bgTasks.add(t);
        }
    }

    private synchronized void onSuccess(Task t) {
        Log.i(LOG_TAG, "Task finished successfully");
    }

    private synchronized void onFailure(Task task) {
        Log.i(LOG_TAG, "Task failed, retrying");
        tasks.add(task);
    }
}
