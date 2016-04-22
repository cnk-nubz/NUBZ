package com.cnk.communication.task;

public abstract class Task {
    public String getTaskName() {
        return this.getClass().getSimpleName();
    }

    public abstract void run();
}
