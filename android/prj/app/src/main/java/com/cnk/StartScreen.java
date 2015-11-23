package com.cnk;

import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;

import com.cnk.communication.NetworkHandler;
import com.cnk.data.DataHandler;
import com.cnk.database.DatabaseHelper;
import com.cnk.ui.MapActivity;

public class StartScreen extends AppCompatActivity {
    NetworkHandler net;
    Button bgButton;
    Button bMapActivity;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        DatabaseHelper dbHelper = new DatabaseHelper(this.getApplicationContext());
        setContentView(R.layout.activity_start_screen);
        DataHandler.getInstance().setContext(getApplication().getApplicationContext());
        DataHandler.getInstance().setDbHelper(dbHelper);
        DataHandler.getInstance().getInitData();
        net = new NetworkHandler();
        bgButton = (Button) findViewById(R.id.bgButton);
        bgButton.setOnClickListener(new BgClick());

        bMapActivity = (Button) findViewById(R.id.bMapActivity);
        bMapActivity.setOnClickListener(new MapActivityClick());
    }

    public void pingClick(View view) {
        net.uploadRaport();
    }

    public void mapClick(View view) {
        net.downloadMap();
    }

    private class BgClick implements View.OnClickListener {
        public void onClick(View v) {
            if (bgButton.getText().equals("Start BG downlaod")) {
                net.startBgDownload();
                bgButton.setText("Stop BG download");
            } else {
                net.stopBgDownload();
                bgButton.setText("Start BG downlaod");
            }

        }
    }

    private class MapActivityClick implements View.OnClickListener {
        @Override
        public void onClick(View view) {
            Intent i = new Intent(getApplicationContext(), MapActivity.class);
            startActivity(i);
        }
    }
}

