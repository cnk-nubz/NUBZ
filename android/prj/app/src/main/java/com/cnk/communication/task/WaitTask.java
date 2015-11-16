package com.cnk.communication.task;

import android.content.Context;

import com.cnk.utilities.Util;

public class WaitTask extends Task {

    private long delay;

    public WaitTask(long delay) {
        this.delay = delay;
    }

    public void run(int tries) {
        Util.waitDelay(delay);
    }

}
