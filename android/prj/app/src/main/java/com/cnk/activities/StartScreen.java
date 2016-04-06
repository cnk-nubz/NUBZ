package com.cnk.activities;

import android.app.ProgressDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.widget.TextView;

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
import com.cnk.ui.Utils;

import java.util.ArrayList;

public class StartScreen extends AppCompatActivity implements Observer {
    private static final String LOG_TAG = "StartScreen";
    private static boolean dataLoaded;
    private DatabaseHelper dbHelper;
    private ProgressDialog spinner;
    private TextView raportsDisplay;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        dbHelper = new DatabaseHelper(this.getApplicationContext());
        setContentView(R.layout.activity_start_screen);
        raportsDisplay = (TextView) findViewById(R.id.raportsStatus);
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
    protected void onResume() {
        super.onResume();
        raportsCountUpdate(ReadyRaports.getInstance().getAllReadyRaports().size());
        ReadyRaports.getInstance().addObserver(this, this::raportsCountUpdate);
    }

    @Override
    protected void onPause() {
        super.onPause();
        ReadyRaports.getInstance().deleteObserver(this);
    }

    @Override
    public void onBackPressed() {

    }

    private void raportsCountUpdate(Integer newCount) {
        raportsDisplay.setText(getString(R.string.raports_queue_size) + " " + newCount.toString());
    }

    public void mapClick(View view) {
        downloadMap();
    }

    public void resetClick(View view) {
        dbHelper.addOrUpdateExhibits(null, new ArrayList<>());
        Log.i(LOG_TAG, "Exhibits reset");
    }

    public void surveyClick(View view) {
        Intent i = new Intent(getApplicationContext(), SurveyActivity.class);
        i.putExtra("nextActivity", MapActivity.class);
        i.putExtra("type", Survey.SurveyType.BEFORE);
        startActivity(i);
    }

    private void downloadMap() {
        setSpinner();
        MapData.getInstance().downloadMap(this::mapDownloaded);
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
                spinner.dismiss();
                e.printStackTrace();
                runOnUiThread(this::showAlert);
            }
        }
        spinner.dismiss();
    }

    private void showAlert() {
        DialogInterface.OnClickListener positiveAction = (dialog, which) -> downloadMap();
        DialogInterface.OnClickListener negativeAction = (dialog, which) -> System.exit(1);
        Utils.showDialog(this,
                         R.string.dataError,
                         R.string.tryAgain,
                         R.string.exit,
                         positiveAction,
                         negativeAction);
    }

    private void setSpinner() {
        spinner = new ProgressDialog(this);
        spinner.setCanceledOnTouchOutside(false);
        spinner.setTitle("Synchronizacja");
        spinner.setMessage("Trwa synchronizacja danych z serwerem");
        spinner.show();
    }
}

