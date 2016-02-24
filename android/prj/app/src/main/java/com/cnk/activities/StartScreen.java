package com.cnk.activities;

import android.app.ProgressDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.support.v7.app.AlertDialog;
import android.support.v7.app.AppCompatActivity;
import android.view.View;

import com.cnk.R;
import com.cnk.communication.NetworkHandler;
import com.cnk.communication.thrift.Survey;
import com.cnk.data.DataHandler;
import com.cnk.database.DatabaseHelper;

import java.util.Observable;
import java.util.Observer;

public class StartScreen extends AppCompatActivity implements Observer {
    private static final int SHOW_ALERT = 1;

    private static Handler uiHandler;
    private ProgressDialog spinner;
    private static boolean dataLoaded;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        this.uiHandler = new UiHandler(Looper.getMainLooper(), this);
        DatabaseHelper dbHelper = new DatabaseHelper(this.getApplicationContext());
        setContentView(R.layout.activity_start_screen);
        DataHandler.getInstance().setDbHelper(dbHelper);
        try {
            if (!dataLoaded) {
                DataHandler.getInstance().loadDbData();
                dataLoaded = true;
            }
        } catch (Exception e) {
            e.printStackTrace();
            downloadMap();
        }
    }

    public void mapClick(View view) {
        downloadMap();
    }

    public void mapActivityClick(View view) {
        Intent i = new Intent(getApplicationContext(), MapActivity.class);
        startActivity(i);
    }

    public void surveyClick(View view) {
        Intent i = new Intent(getApplicationContext(), SurveyActivity.class);
        startActivity(i);
    }

    public void update(Observable observable, Object o) {
        if (o.equals(DataHandler.Item.MAP_UPDATE_COMPLETED)) {
            if (!dataLoaded) {
                try {
                    DataHandler.getInstance().loadDbData();
                    dataLoaded = true;
                } catch (Exception e) {
                    Looper.prepare();
                    spinner.dismiss();
                    DataHandler.getInstance().deleteObserver(this);
                    e.printStackTrace();
                    Message msg = uiHandler.obtainMessage(SHOW_ALERT);
                    msg.sendToTarget();
                }
            }
            spinner.dismiss();
            DataHandler.getInstance().deleteObserver(this);
        }
    }

    public void showAlert() {
        AlertDialog.Builder alert = new AlertDialog.Builder(this);
        alert.setTitle(R.string.error);
        alert.setMessage(R.string.DataError);
        alert.setPositiveButton("Spróbuj ponownie", new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                downloadMap();
            }
        });
        alert.setNegativeButton("Wyjdź", new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                System.exit(1);
            }
        });
        alert.setCancelable(false);
        alert.show();
    }

    private void downloadMap() {
        DataHandler.getInstance().addObserver(this);
        setSpinner();
        NetworkHandler.getInstance().downloadMap();
    }

    private void setSpinner() {
        spinner = new ProgressDialog(this);
        spinner.setCanceledOnTouchOutside(false);
        spinner.setTitle("Synchronizacja");
        spinner.setMessage("Trwa synchronizacja danych z serwerem");
        spinner.show();
    }

    private static class UiHandler extends Handler {
        private StartScreen activity;
        public UiHandler(Looper looper, StartScreen activity) {
            super(looper);
            this.activity = activity;
        }
        @Override
        public void handleMessage(Message message) {
            if (message.what == SHOW_ALERT) {
                activity.showAlert();
            }
        }
    }
}

