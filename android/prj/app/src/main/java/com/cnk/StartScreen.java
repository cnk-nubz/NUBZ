package com.cnk;

import android.app.Application;
import android.app.ProgressDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;
import android.provider.ContactsContract;
import android.support.v7.app.AlertDialog;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.Toast;

import com.cnk.communication.NetworkHandler;
import com.cnk.data.DataHandler;
import com.cnk.database.DatabaseHelper;
import com.cnk.ui.MapActivity;

import java.util.Observable;
import java.util.Observer;

public class StartScreen extends AppCompatActivity implements Observer {
    private ProgressDialog spinner;
    private static boolean dataLoaded;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
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

    public void update(Observable observable, Object o) {
        if (o.equals(DataHandler.Item.MAP_UPDATE_COMPLETED)) {
            if (!dataLoaded) {
                try {
                    DataHandler.getInstance().loadDbData();
                } catch (Exception e) {
                    e.printStackTrace();
                    showAlert();
                }

                dataLoaded = true;
            }
            spinner.dismiss();
            DataHandler.getInstance().deleteObserver(this);
        }
    }

    public void mapActivityClick(View view) {
        Intent i = new Intent(getApplicationContext(), MapActivity.class);
        startActivity(i);
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

    private void showAlert() {
        AlertDialog.Builder alert = new AlertDialog.Builder(this);
        alert.setTitle(R.string.error);
        alert.setMessage(R.string.DataError);
        alert.setPositiveButton("OK", new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                System.exit(1);
            }
        });
        alert.setCancelable(false);
        alert.show();
    }
}

