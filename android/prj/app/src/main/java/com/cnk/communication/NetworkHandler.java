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
    private Context context;
    private Notificator mapDownload;
    private Notificator raportUpload;
    private Thread t;

    private class QueueThread extends Thread {
        private static final int TRIES = 3;
        public void run() {
            while(true) {
                synchronized(this) {
                    if (!tasks.isEmpty()) {
                        tasks.remove().run(3);
                    } else {
                        try {
                            this.wait();
                        } catch (InterruptedException e) {
                            e.printStackTrace();
                        }
                    }
                }
            }
        }
    }

    public NetworkHandler(Context context) {
        this.context = context;

        mapDownload = new Notificator();
        raportUpload = new Notificator();
        mapDownload.addObserver(this);
        raportUpload.addObserver(this);
        MapDownloader.setNotificator(mapDownload);
        RaportUploader.setNotificator(raportUpload);

        tasks = new ConcurrentLinkedQueue<>();
        t = new QueueThread();
        t.start();
    }

    public synchronized void downloadMap() {
        synchronized(t) {
            Task task = new MapDownloadTask(context);
            tasks.add(task);
            t.notify();
        }
    }

    public synchronized void uploadRaport() {
        synchronized(t) {
            Task task = new RaportUploadTask(context);
            tasks.add(task);
            t.notify();
        }
    }

    public synchronized void addWaitTask() {
        synchronized(t) {
            Task task = new WaitTask(context, LONG_DELAY);
            tasks.add(task);
            t.notify();
        }
    }

    public void update(Observable o, Object arg) {
        addWaitTask();
        if (o == mapDownload) {
            downloadMap();
        } else if (o == raportUpload) {
            uploadRaport();
        }
    }

}
