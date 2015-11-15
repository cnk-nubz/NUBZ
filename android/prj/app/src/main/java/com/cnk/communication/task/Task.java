package com.cnk.communication.task;

import android.content.Context;

public abstract class Task {
    protected Context context;

    public Task(Context context) {
        this.context = context;
    }

    public abstract void run(int tries);
}
