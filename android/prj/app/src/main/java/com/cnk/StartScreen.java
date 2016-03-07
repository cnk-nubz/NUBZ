package com.cnk;

import android.app.ProgressDialog;
import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;

import com.cnk.communication.NetworkHandler;
import com.cnk.data.DataHandler;
import com.cnk.database.DatabaseHelper;
import com.cnk.ui.MapActivity;

import java.io.IOException;
import java.util.Observable;
import java.util.Observer;

public class StartScreen extends AppCompatActivity implements Observer {
    private NetworkHandler net;
    private ProgressDialog spinner;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        DatabaseHelper dbHelper = new DatabaseHelper(this.getApplicationContext());
        setContentView(R.layout.activity_start_screen);
        DataHandler.getInstance().setDbHelper(dbHelper);
        net = new NetworkHandler();
    }

    public void mapClick(View view) {
        DataHandler.getInstance().addObserver(this);
        setSpinner();
        net.downloadMap();
    }

    public void update(Observable observable, Object o) {
        if (o.equals(DataHandler.Item.MAP_UPDATE_COMPLETED)) {
            spinner.dismiss();
            DataHandler.getInstance().deleteObserver(this);
        }
    }

    public void mapActivityClick(View view) {
        //TODO only for testing:
        try {
            DataHandler.getInstance().startNewRaport();
        } catch (IOException e) {
            e.printStackTrace();
        }

        Intent i = new Intent(getApplicationContext(), MapActivity.class);
        startActivity(i);
    }

    private void setSpinner() {
        spinner = new ProgressDialog(this);
        spinner.setCanceledOnTouchOutside(false);
        spinner.setTitle("Synchronizacja");
        spinner.setMessage("Trwa synchronizacja danych z serwerem");
        spinner.show();
    }
}

