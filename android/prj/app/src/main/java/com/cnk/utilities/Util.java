package com.cnk.utilities;

public class Util {

    public static void waitDelay(long delay) {
        try {
            Thread.sleep(delay);
        } catch (java.lang.InterruptedException e2) {
            e2.printStackTrace();
        }
    }

}
