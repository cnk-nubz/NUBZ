package com.cnk.communication.task;

public abstract class Task {
    public interface TimeoutAction {
        void onTimeout();
    }

    public abstract void run(int tries);
    public abstract long getTimeout();
    public abstract Long getBeginTime();
    public abstract TimeoutAction getTimeoutAction();
}
