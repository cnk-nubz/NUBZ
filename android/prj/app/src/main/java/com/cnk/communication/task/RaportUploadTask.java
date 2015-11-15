package com.cnk.communication.task;

import android.content.Context;
import android.content.Intent;

import com.cnk.communication.RaportUploader;

public class RaportUploadTask extends Task {

    public RaportUploadTask(Context context) {
        super(context);
    }

    public void run(int tries) {
        Intent intent = new Intent(context, RaportUploader.class);
        intent.putExtra("tries", tries);
        context.startService(intent);
    }

}
