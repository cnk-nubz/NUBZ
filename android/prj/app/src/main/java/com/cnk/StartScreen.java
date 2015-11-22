package com.cnk;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;

import com.cnk.communication.NetworkHandler;
import com.cnk.data.DataHandler;
import com.cnk.database.DatabaseHelper;

public class StartScreen extends AppCompatActivity {
    NetworkHandler net;
    Button bgButton;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        DatabaseHelper dbHelper = new DatabaseHelper(this.getApplicationContext());
        setContentView(R.layout.activity_start_screen);
        DataHandler.getInstance().setContext(getApplication().getApplicationContext());
        DataHandler.getInstance().setDbHelper(dbHelper);
        DataHandler.getInstance().getInitData();
        ImageView v = (ImageView) findViewById(R.id.imageView);
        if (DataHandler.getInstance().getFloorMap(0) != null) {
            v.setBackground(DataHandler.getInstance().getFloorMap(0));
        }
        net = new NetworkHandler();
        bgButton = (Button) findViewById(R.id.bgButton);
        bgButton.setOnClickListener(new BgClick());
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
}

