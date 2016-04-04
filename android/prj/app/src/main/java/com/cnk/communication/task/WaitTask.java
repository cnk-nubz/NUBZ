package com.cnk.communication.task;

import com.cnk.utilities.Consts;
import com.cnk.utilities.Util;

import java.util.Date;

public class WaitTask extends Task {

    private static final long
            TIMEOUT =
            Consts.MILLIS_IN_SEC * Consts.SEC_IN_MIN * Consts.SEC_IN_MIN * Consts.MIN_IN_H *
            Consts.H_IN_DAY * Consts.DAYS_IN_MONTH;
    private long delay;
    private Long beginTime;

    public WaitTask(long delay) {
        this.delay = delay;
    }

    public void run(int tries) {
        if (beginTime == null) {
            beginTime = new Date().getTime();
        }
        Util.waitDelay(delay);
    }

    @Override
    public long getTimeout() {
        return TIMEOUT;
    }

    @Override
    public Long getBeginTime() {
        return beginTime;
    }

    @Override
    public TimeoutAction getTimeoutAction() {
        return null;
    }

}
