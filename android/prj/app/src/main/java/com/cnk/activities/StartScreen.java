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
import com.cnk.data.exhibits.ExhibitsData;
import com.cnk.data.experiment.ExperimentData;
import com.cnk.data.experiment.survey.Survey;
import com.cnk.data.map.MapData;
import com.cnk.data.raports.ReadyRaports;
import com.cnk.database.DatabaseHelper;
import com.cnk.exceptions.DatabaseLoadException;
import com.cnk.notificators.Observer;

public class StartScreen extends AppCompatActivity implements Observer {
    private static final int SHOW_ALERT = 1;

    private static Handler uiHandler;
    private static boolean dataLoaded;
    private ProgressDialog spinner;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        uiHandler = new UiHandler(Looper.getMainLooper(), this);
        DatabaseHelper dbHelper = new DatabaseHelper(this.getApplicationContext());
        setContentView(R.layout.activity_start_screen);
        ExperimentData.getInstance().setDbHelper(dbHelper);
        MapData.getInstance().setDbHelper(dbHelper);
        ExhibitsData.getInstance().setDbHelper(dbHelper);
        ReadyRaports.getInstance().setDbHelper(dbHelper);
        try {
            if (!dataLoaded) {
                MapData.getInstance().loadDbData();
                ExhibitsData.getInstance().loadDbData();
                ReadyRaports.getInstance().loadDbData();
                NetworkHandler.getInstance().uploadRaports();
                dataLoaded = true;
            }
        } catch (DatabaseLoadException e) {
            e.printStackTrace();
            downloadMap();
        }
    }

    @Override
    public void onBackPressed() {

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
        i.putExtra("nextActivity", MapActivity.class);
        i.putExtra("type", Survey.SurveyType.BEFORE);
        startActivity(i);
    }

    public void showAlert() {
        AlertDialog.Builder alert = new AlertDialog.Builder(this);
        alert.setTitle(R.string.error);
        alert.setMessage(R.string.dataError);
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
        MapData.getInstance().addObserver(this, this::mapDownloaded);
        setSpinner();
        NetworkHandler.getInstance().downloadMap();
    }

    private void mapDownloaded() {
        if (!dataLoaded) {
            try {
                MapData.getInstance().loadDbData();
                ExhibitsData.getInstance().loadDbData();
                ReadyRaports.getInstance().loadDbData();
                NetworkHandler.getInstance().uploadRaports();
                dataLoaded = true;
            } catch (DatabaseLoadException e) {
                Looper.prepare();
                spinner.dismiss();
                MapData.getInstance().deleteObserver(this);
                e.printStackTrace();
                Message msg = uiHandler.obtainMessage(SHOW_ALERT);
                msg.sendToTarget();
            }
        }
        spinner.dismiss();
        MapData.getInstance().deleteObserver(this);
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

