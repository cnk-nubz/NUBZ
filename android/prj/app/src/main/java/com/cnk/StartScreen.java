package com.cnk;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;

import com.cnk.R;
import com.cnk.communication.NetworkHandler;

import java.io.File;

public class StartScreen extends AppCompatActivity {

    NetworkHandler net;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_start_screen);
        net = new NetworkHandler(this.getBaseContext());
        net.scanForRaports();
    }

    public void pingClick(View view) {
        net.addRaportToSend(new File("testFile"));
    }
}

