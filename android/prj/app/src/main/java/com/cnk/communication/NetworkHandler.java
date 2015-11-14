package com.cnk.communication;

import android.content.Context;
import android.content.Intent;

import com.cnk.data.DataHandler;

import java.io.File;
import java.util.Queue;
import java.util.concurrent.LinkedBlockingQueue;

public class NetworkHandler {
    private static final String SEND_ADDRESS = "192.168.1.111";
    private static final int SEND_PORT = 9090;
    private static final long delay = 300;
    private static boolean download;
    private Queue<File> raports;
    private Context context;
    private DataHandler dataHandler;

    public NetworkHandler(Context context, DataHandler dataHandler) {
        this.context = context;
        this.dataHandler = dataHandler;
        raports = new LinkedBlockingQueue<>();
        download = false;
        RaportUploader.setEndpoint(SEND_ADDRESS, SEND_PORT);
        MapDownloader.setEndpoint(SEND_ADDRESS, SEND_PORT);
        MapDownloader.setDataHandler(dataHandler);
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

    public void downloadMap() {
        Intent intent = new Intent(context, MapDownloader.class);
        context.startService(intent);
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
            downloadMap();
            downloadData();
        }
    }

}
