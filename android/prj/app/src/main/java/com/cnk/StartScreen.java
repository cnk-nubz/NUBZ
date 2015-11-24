package com.cnk;

import android.content.Intent;
import android.os.AsyncTask;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.widget.Button;

import com.cnk.communication.NetworkHandler;
import com.cnk.data.DataHandler;
import com.cnk.database.DatabaseHelper;
import com.cnk.database.Exhibit;
import com.cnk.ui.MapActivity;

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
        DataHandler.getInstance().setContext(getApplication().getApplicationContext());
        DataHandler.getInstance().setDbHelper(dbHelper);
        net = new NetworkHandler();

        bMapActivity = (Button) findViewById(R.id.bMapActivity);
        bMapActivity.setOnClickListener(new MapActivityClick());
    }

    public void pingClick(View view) {
        net.uploadRaport();
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

    public void databaseTest(View view) {
        AsyncTask<Integer, Void, Void> dbTestTask = new AsyncTask<Integer, Void, Void>() {
            @Override
            protected Void doInBackground(Integer... integers) {
                DatabaseHelper dbHelper = new DatabaseHelper(getApplicationContext());

                ArrayList<Exhibit> exhibits = new ArrayList<>();
                List<Exhibit> exhibitList;
                Log.i("Creating dummy exhibits", "begin");
                for (int i = 1; i <= integers[0]; i++) {
                    exhibits.add(new Exhibit(i, 10, 10, 10, 10, 1, "asdf asdf asdf asdf asdf"));
                }
                Log.i("Creating dummy exhibits", "end");

                Log.i("Save dummy exhibits", "begin");
                dbHelper.setAllExhibits(exhibits);
                Log.i("Save dummy exhibits", "end");

                Log.i("Save dummy floor", "begin");
                dbHelper.setExhibitsForFloor(exhibits, 1);
                Log.i("Save dummy floor", "end");

                Log.i("Get all dummy", "begin");
                exhibitList = dbHelper.getAllExhibits();
                Log.i("Get all dummy", "end");

                Log.i("Get all dummy for floor", "begin");
                exhibitList = dbHelper.getAllExhibitsForFloor(1);
                Log.i("Get all dummy for floor", "end");

                Exhibit e = dbHelper.getExhibit(1);

                return null;
            }
        };

        dbTestTask.execute(1000);
    }
}

