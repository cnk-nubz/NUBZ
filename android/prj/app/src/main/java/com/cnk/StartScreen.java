package com.cnk;

import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.widget.Button;

import com.cnk.communication.NetworkHandler;
import com.cnk.data.Action;
import com.cnk.data.DataHandler;
import com.cnk.data.RaportEvent;
import com.cnk.database.DatabaseHelper;
import com.cnk.ui.MapActivity;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

public class StartScreen extends AppCompatActivity {
    NetworkHandler net;
    Button bMapActivity;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        DatabaseHelper dbHelper = new DatabaseHelper(this.getApplicationContext());
        setContentView(R.layout.activity_start_screen);
        DataHandler.getInstance().setDbHelper(dbHelper);
        net = new NetworkHandler();
        net.downloadExperimentData();
        bMapActivity = (Button) findViewById(R.id.bMapActivity);
        bMapActivity.setOnClickListener(new MapActivityClick());
    }

    public void testClick(View view) {
        List<Action> actions = DataHandler.getInstance().getAllExhibitActions();
        for (Action a : actions) {
            Log.i("TEST", a.toString());
        }
    }

    public void mapClick(View view) {
        net.downloadMap();
    }

    private class MapActivityClick implements View.OnClickListener {
        @Override
        public void onClick(View view) {
            Intent i = new Intent(getApplicationContext(), MapActivity.class);
            startActivity(i);
        }
    }
}

