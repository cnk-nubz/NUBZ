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


    private class QueueThread extends Thread {
        private static final int TRIES = 3;
        public void run() {
            while(true) {
                if (!tasks.isEmpty()) {
                    tasks.remove().run(3);
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
        (new QueueThread()).start();
    }

    public void downloadMap() {
        Task t = new MapDownloadTask(context);
        tasks.add(t);
    }

    public void uploadRaport() {
        Task t = new RaportUploadTask(context);
        tasks.add(t);
    }

    public void addWaitTask() {
        Task t = new WaitTask(context, LONG_DELAY);
        tasks.add(t);
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
