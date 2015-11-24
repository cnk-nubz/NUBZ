package com.cnk;

import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Button;

import com.cnk.communication.NetworkHandler;
import com.cnk.data.DataHandler;
import com.cnk.database.DatabaseHelper;
import com.cnk.ui.MapActivity;

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
}

