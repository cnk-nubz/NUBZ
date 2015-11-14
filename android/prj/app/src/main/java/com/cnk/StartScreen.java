package com.cnk;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;

import com.cnk.data.DataHandler;
import com.cnk.communication.NetworkHandler;

import java.io.File;

public class StartScreen extends AppCompatActivity {

    NetworkHandler net;
    DataHandler data;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_start_screen);
        data = new DataHandler(this.getBaseContext());
        net = new NetworkHandler(this.getBaseContext(), data);
        net.scanForRaports();
    }

    public void pingClick(View view) {
        net.addRaportToSend(new File("testFile"));
    }
    public void mapClick(View view) {
        net.downloadMap();
    }
}

