package com.cnk.data;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;
import android.util.Log;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.Map;
import java.util.Observable;
import java.util.concurrent.ConcurrentHashMap;

public class DataHandler extends Observable {

    // private Map<Integer, Exhibit> exhibits;
    private static final String LOG_TAG = "DataHandler";
    private static final String CONFIG_FILE = "config";
    private Map<Integer, File> mapFiles;
    private Map<Integer, Drawable> mapDrawables;
    private DataConfig config;
    private Context context;

    public DataHandler(Context context) {
        //exhibits = new HashMap<>();
        this.context = context;
        mapDrawables = new ConcurrentHashMap<>();
        mapFiles = new ConcurrentHashMap<>();
        loadConfig();
    }

    public Map<Integer, Drawable> getMap() {
        return mapDrawables;
    }

    public Drawable getFloorMap(Integer floor) {
        return mapDrawables.get(floor);
    }

    public void setMapFloor(int floor, String url) throws IOException {
        Drawable image = urlToDrawable(url);
        mapDrawables.put(floor, image);
    }

    public void setMapVersion(int newVersion) {
        config.setMapVersion(newVersion);
        saveConfig();
    }

    public int getMapVersion() {
        return config.getMapVersion();
    }

    private void createNewFile() {
        config = new DataConfig(0, 0);
        saveConfig();
    }

    private void loadConfig() {
        Log.i(LOG_TAG, "looking for file");
        try {
            FileInputStream in = context.openFileInput(CONFIG_FILE);
            ObjectInputStream objStream = new ObjectInputStream(in);
            config = (DataConfig) objStream.readObject();
            in.close();
            objStream.close();
            Log.i(LOG_TAG, "finding successful");
        } catch (IOException e1) {
            Log.e(LOG_TAG, "error with file, creating new one");
            createNewFile();
            e1.printStackTrace();
        } catch (ClassNotFoundException e2) {
            Log.e(LOG_TAG, "class not found");
            e2.printStackTrace();
        }
    }

    private void saveConfig() {
        try {
            FileOutputStream out = context.openFileOutput(CONFIG_FILE, Context.MODE_PRIVATE);
            ObjectOutputStream objectStream = new ObjectOutputStream(out);
            objectStream.writeObject(config);
            out.close();
            objectStream.close();
            Log.i(LOG_TAG, "saving config file");
        } catch (IOException e) {
            e.printStackTrace();
            Log.e(LOG_TAG, "cannot save config file");
        }
    }

    private Drawable urlToDrawable(String url) throws IOException {
        Bitmap bmp;
        HttpURLConnection connection = (HttpURLConnection) new URL(url).openConnection();
        connection.connect();
        InputStream input = connection.getInputStream();
        bmp = BitmapFactory.decodeStream(input);
        return new BitmapDrawable(context.getResources(), bmp);
    }

    private void scanForMapFiles() {

    }

}
