package com.cnk.communication;

import android.content.Context;
import android.content.Intent;

import java.io.File;
import java.util.Queue;
import java.util.concurrent.LinkedBlockingQueue;

public class NetworkHandler {
    // private static final String RECEIVE__ADDRESS = "";
    // private static final int RECEIVE_PORT = 1;
    private static final long delay = 300;
    private static boolean download;
    // private DataDownloader downloader;
    // private Data currentData;
    private Queue<File> raports;
    private Context context;

    public NetworkHandler(Context context) {
        this.context = context;
        raports = new LinkedBlockingQueue<>();
        download = false;
        //downloader = new DataDownloader(SERVER_ADDRESS, SERVER_PORT);
        //currentData = null;
    }

    public void sendRaports() {
        while (!raports.isEmpty()) {
            Intent intent = new Intent(context, RaportUploader.class);
            intent.putExtra("Raport", raports.remove());
            context.startService(intent);
        }
    }

    public void startBackgroundDownload() {
        download = true;
        downloadDataInBackground();
    }

    public void stopBackgroundDownload() {
        download = false;
    }

    public void downloadData() {
        Intent intent = new Intent();
        // downloader.startService(intent)
        try {
            wait(delay);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        // currentData = downloader.getData();
    }

    public /* Data */ void getCurrentData() {
        // return currentData;
    }

    public void addRaportToSend(File raport) {
        raports.add(raport);
        sendRaports();
    }

    public void scanForRaports() {

        //TODO scanning the directory for raport files
        sendRaports();
    }

    private void downloadDataInBackground() {
        while (download) {
            downloadData();
        }
    }

}
