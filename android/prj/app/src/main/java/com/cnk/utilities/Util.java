package com.cnk.utilities;

import java.util.Date;

public class Util {

    public static void waitDelay(long delay) {
        try {
            Thread.sleep(delay * Consts.MILLIS_IN_SEC);
        } catch (java.lang.InterruptedException e2) {
            e2.printStackTrace();
        }
    }

    public static boolean checkIfBeforeTimeout(long beginTime, long timeout) {
        return new Date().getTime() - beginTime < timeout;
    }

}
