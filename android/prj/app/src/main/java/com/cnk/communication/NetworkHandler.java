package com.cnk.communication;

import android.content.Context;
import android.content.Intent;
import android.util.Log;

import com.cnk.communication.task.MapDownloadTask;
import com.cnk.communication.task.RaportUploadTask;
import com.cnk.communication.task.Task;
import com.cnk.communication.task.WaitTask;
import com.cnk.data.DataHandler;
import com.cnk.notificators.Notificator;
import com.cnk.utilities.Util;

import java.io.File;
import java.io.IOError;
import java.util.Observable;
import java.util.Observer;
import java.util.Queue;
import java.util.concurrent.ConcurrentLinkedQueue;
import java.util.concurrent.LinkedBlockingQueue;

public class NetworkHandler implements Observer {

    private static final String LOG_TAG = "NetworkHandler";
    private static final long DELAY = 30 * 1000;
    private static final long LONG_DELAY = 300 * 1000;

    private Queue<Task> tasks;
    private Notificator mapDownload;
    private Notificator raportUpload;
    private Thread queuer;

    private class QueueThread extends Thread {
        private static final int TRIES = 3;
        public void run() {
            while(true) {
                if (!tasks.isEmpty()) {
                    Log.i(LOG_TAG, "starting another task");
                    tasks.remove().run(3);
                    Log.i(LOG_TAG, "task finished");
                } else {
                    synchronized (this) {
                        try {
                            Log.i(LOG_TAG, "no tasks to run, sleeping");
                            this.wait();
                        } catch (InterruptedException e) {
                            e.printStackTrace();
                            Log.i(LOG_TAG, "resuming work");
                        }
                    }
                }
            }
        }
    }

    public NetworkHandler() {
        mapDownload = new Notificator();
        raportUpload = new Notificator();
        mapDownload.addObserver(this);
        raportUpload.addObserver(this);
        tasks = new ConcurrentLinkedQueue<>();
        queuer = new QueueThread();
        queuer.start();
    }

    public synchronized void downloadMap() {
        synchronized(queuer) {
            Task task = new MapDownloadTask(mapDownload);
            tasks.add(task);
            queuer.notify();
        }
    }

    public synchronized void uploadRaport() {
        synchronized(queuer) {
            Task task = new RaportUploadTask(raportUpload, null);
            tasks.add(task);
            queuer.notify();
        }
    }

    public synchronized void addWaitTask() {
        synchronized(queuer) {
            Task task = new WaitTask(LONG_DELAY);
            tasks.add(task);
            queuer.notify();
        }
    }

    public void update(Observable o, Object arg) {
        if ((boolean) arg == false) {
            onFailure(o);
        } else {
            onSuccess(o);
        }
    }

    private void onSuccess(Observable o) {
        Log.i(LOG_TAG, "task finished successfully");
    }

    private void onFailure(Observable o) {
        Log.i(LOG_TAG, "task failed, retrying");
        /*
        addWaitTask();
        if (o == mapDownload) {
            downloadMap();
        } else if (o == raportUpload) {
            uploadRaport();
        }
        */
    }

}
