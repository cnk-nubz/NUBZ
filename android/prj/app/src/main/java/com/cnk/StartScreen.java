package com.cnk;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;

import com.cnk.data.DataHandler;
import com.cnk.communication.NetworkHandler;


public class StartScreen extends AppCompatActivity {

    NetworkHandler net;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_start_screen);
        DataHandler.setContext(getApplication().getApplicationContext());
        net = new NetworkHandler();
    }

    public void pingClick(View view) {
        net.uploadRaport();
    }
    public void mapClick(View view) {
        net.downloadMap();
    }
}

