package com.cnk;

import android.app.Application;

import com.cnk.communication.NetworkHandler;

public class Cnk extends Application {

    @Override
    public void onCreate() {
        super.onCreate();
        NetworkHandler nh = NetworkHandler.getInstance();
        nh.setAppContext(getApplicationContext());
        nh.startBgDataSync();
    }
}
