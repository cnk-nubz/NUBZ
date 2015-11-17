package com.cnk;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;

import com.cnk.R;
import com.cnk.communication.NetworkHandler;
import com.cnk.database.DatabaseHelper;

import java.io.File;

public class StartScreen extends AppCompatActivity {

    NetworkHandler net;
    Button bDatabaseTest;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_start_screen);
        net = new NetworkHandler(this.getBaseContext());
        net.scanForRaports();

        bDatabaseTest = (Button) findViewById(R.id.bDatabaseTest);
        bDatabaseTest.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                DatabaseHelper helper = new DatabaseHelper(getApplicationContext());
                helper.open();

                helper.setVersion("testItem1", 1);
                helper.setMapFile(1000, "/asdf/asdfFile");
                if (helper.getVersion("testItem1").getCurrentVersion() == 1 &&
                        helper.getMapFile(1000).getMapFileLocation().equals("/asdf/asdfFile")) {
                    Toast.makeText(StartScreen.this, "Database OK", Toast.LENGTH_SHORT).show();
                } else {
                    Toast.makeText(StartScreen.this, "Database FAILED", Toast.LENGTH_SHORT).show();
                }
            }
        });
    }

    public void pingClick(View view) {
        net.addRaportToSend(new File("testFile"));
    }
}

