package com.cnk.communication.task;

import android.content.Context;
import android.content.Intent;

import com.cnk.communication.MapDownloader;

public class MapDownloadTask extends Task {

    public MapDownloadTask(Context context) {
        super(context);
    }

    public void run(int tries) {
        Intent intent = new Intent(context, MapDownloader.class);
        intent.putExtra("tries", tries);
        context.startService(intent);
    }
}
