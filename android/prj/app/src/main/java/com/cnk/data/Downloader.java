package com.cnk.data;

import android.util.Log;

import java.io.IOException;
import java.io.InputStream;
import java.net.URL;

public class Downloader {

    private static final String PROTOCOL = "http://";
    private static final String LOG_TAG = "Downloader";
    private static Downloader instance;

    private Downloader() {
    }

    public static Downloader getInstance() {
        if (instance == null) {
            instance = new Downloader();
        }
        return instance;
    }

    public InputStream download(String url) throws IOException {
        URL fileUrl = new URL(PROTOCOL + url);
        Log.i(LOG_TAG, "Downloading " + url);
        return fileUrl.openStream();
    }
}
