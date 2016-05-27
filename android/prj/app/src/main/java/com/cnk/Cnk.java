package com.cnk;

import android.app.Application;
import android.content.Context;

import com.cnk.communication.NetworkHandler;

public class Cnk extends Application {

    private static Cnk instance;

    public Cnk() {
        instance = this;
    }


    @Override
    public void onCreate() {
        super.onCreate();

        NetworkHandler nh = NetworkHandler.getInstance();
        nh.setAppContext(getApplicationContext());
    }

    public static Context getAppContext() {
        return instance;
    }
}
