package com.cnk.communication.task;

import android.content.Context;

import com.cnk.utilities.Util;

public class WaitTask extends Task {

    private long delay;

    public WaitTask(Context context, long delay) {
        super(context);
        this.delay = delay;
    }

    public void run(int tries) {
        Util.waitDelay(delay);
    }

}
