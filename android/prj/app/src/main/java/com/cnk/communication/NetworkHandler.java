package com.cnk.communication;

import android.util.Log;

import com.cnk.communication.task.BackgroundDownloadTask;
import com.cnk.communication.task.ExhibitDownloadTask;
import com.cnk.communication.task.MapDownloadTask;
import com.cnk.communication.task.RaportUploadTask;
import com.cnk.communication.task.Task;
import com.cnk.communication.task.WaitTask;
import com.cnk.notificators.Notificator;

import java.util.Observable;
import java.util.Observer;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.LinkedBlockingQueue;

public class NetworkHandler implements Observer {

    private static final String LOG_TAG = "NetworkHandler";
    private static final long SECONDS_DELAY = 30;

    private long bgDelaySeconds;
    private BlockingQueue<Task> tasks;
    private BlockingQueue<Task> bgTasks;
    private Notificator mapDownload;
    private Notificator raportUpload;
    private Notificator exhibitsDownload;
    private Notificator bgDownload;
    private Notificator bgRaportUpload;
    private boolean downloadInBg;
    private boolean uploadInBg;

    private class QueueThread implements Runnable {
        private static final int TRIES = 3;
        private BlockingQueue<Task> queue;
        public QueueThread(BlockingQueue<Task> queue) {
            this.queue = queue;
        }
        public void run() {
            while(true) {
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

    public NetworkHandler() {
        mapDownload = new Notificator(this);
        raportUpload = new Notificator(this);
        exhibitsDownload = new Notificator(this);
        bgRaportUpload = new Notificator(this);
        bgDownload = new Notificator(this);
        downloadInBg = false;
        bgDelaySeconds = 30;
        tasks = new LinkedBlockingQueue<>();
        bgTasks = new LinkedBlockingQueue<>();
        Thread onDemendQueuer = new Thread(new QueueThread(tasks));
        Thread bgQueuer = new Thread(new QueueThread(bgTasks));
        bgQueuer.start();
        onDemendQueuer.start();
    }

    public synchronized void downloadMap() {
        Task task = new MapDownloadTask(mapDownload);
        tasks.add(task);
    }

    public synchronized void uploadRaport() {
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

    public synchronized void startBgUpload() {
        if (uploadInBg) {
            return;
        }
        uploadInBg = true;
        addBgUploadTask();
    }

    public synchronized void stopBgUpload() {
        uploadInBg = false;
    }

    public void update(Observable o, Object arg) {
        if (o == bgDownload && downloadInBg) {
            addBgDownloadTask();
        } else if (!(boolean) arg) {
            onFailure(o);
        } else {
            onSuccess(o);
        }
    }

    public void setSecondsDelay(long seconds) {
        bgDelaySeconds = seconds;
    }

    public long getSecondsDelay() {
        return bgDelaySeconds;
    }

    private synchronized void addWaitTask() {
        Task task = new WaitTask(SECONDS_DELAY * 1000);
        tasks.add(task);
    }

    private synchronized void addBgUploadTask() {
        Task task = new RaportUploadTask(bgRaportUpload);
        Task wait = new WaitTask(bgDelaySeconds * 1000);
        bgTasks.add(task);
        bgTasks.add(wait);
    }

    private synchronized void addBgDownloadTask() {
        Task task = new BackgroundDownloadTask(bgDownload);
        Task wait = new WaitTask(bgDelaySeconds * 1000);
        bgTasks.add(task);
        bgTasks.add(wait);
    }

    private synchronized void onSuccess(Observable o) {
        Log.i(LOG_TAG, "Task finished successfully");
        if (downloadInBg && o == bgDownload) {
            addBgDownloadTask();
        }
    }

    private synchronized void onFailure(Observable o) {
        Log.i(LOG_TAG, "Task failed, retrying");
        //addWaitTask();
        if (o == mapDownload) {
            Log.e(LOG_TAG, "Map download task failure");
            downloadMap();
        } else if (o == raportUpload) {
            Log.e(LOG_TAG, "Raport upload task failure");
            uploadRaport();
        } else if (o == exhibitsDownload) {
            Log.e(LOG_TAG, "Exhibits download task failed");
            downloadExhibits();
        }
    }

}
