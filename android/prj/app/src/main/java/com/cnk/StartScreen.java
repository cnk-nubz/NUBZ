package com.cnk;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;

import com.cnk.R;
import com.cnk.communication.NetworkHandler;
import com.cnk.database.DatabaseHelper;
import com.cnk.database.Version;

import org.javatuples.Triplet;

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

                Integer vCode = 1;
                String map1Loc = "asdf1";
                String map2Loc = "asdf2";

                try {
                    helper.setMap(vCode, map1Loc, map2Loc);
                    Triplet<Integer, String, String> res = helper.getCurrentMapData();
                    if (res.getValue0().equals(vCode) && res.getValue1().equals(map1Loc) && res.getValue2().equals(map2Loc)) {
                        Toast.makeText(StartScreen.this, "DATABASE OK", Toast.LENGTH_SHORT).show();
                    } else {
                        Toast.makeText(StartScreen.this, "DATABASE DOESNT WORK PROPERLY", Toast.LENGTH_SHORT).show();
                    }
                } catch (Exception e) {
                    Log.e("Database error", "error msg: " + e.toString());
                    Toast.makeText(StartScreen.this, "DATABASE EXCEPTION", Toast.LENGTH_SHORT).show();
                }

                helper.close();
            }
        });
    }

    public void pingClick(View view) {
        net.addRaportToSend(new File("testFile"));
    }
}

