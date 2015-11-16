package com.cnk.data;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;
import android.util.Log;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.io.OutputStream;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.Map;
import java.util.Observable;
import java.util.concurrent.ConcurrentHashMap;

public class DataHandler extends Observable {

    // private Map<Integer, Exhibit> exhibits;
    private static final String LOG_TAG = "DataHandler";
    private static final String CONFIG_FILE = "config";
    private static final String MAP_FILE_PREFIX = "map";
    private static DataHandler instance;
    private static Context context;
    private Map<Integer, File> mapFiles;
    private Map<Integer, Drawable> mapDrawables;
    private DataConfig config;

    public static DataHandler getInstance() {
        if (instance == null) {
            instance = new DataHandler();
        }
        return instance;
    }

    public static void setContext(Context c) {
        context = c;
    }

    private DataHandler() {
        //exhibits = new HashMap<>();
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
        Drawable image = urlToDrawable(url, floor);
        mapDrawables.put(floor, image);
        setChanged();
        notifyObservers();
    }

    public void setMapVersion(int newVersion) {
        config.setMapVersion(newVersion);
        saveConfig();
    }

    public Integer getMapVersion() {
        return config.getMapVersion();
    }

    private void createNewFile() {
        config = new DataConfig(null, null);
        saveConfig();
    }

    private void loadConfig() {
        Log.i(LOG_TAG, "looking for file");
        try {
            FileInputStream in = context.openFileInput(CONFIG_FILE);
            ObjectInputStream objStream = new ObjectInputStream(in);
            config = (DataConfig) objStream.readObject();
            objStream.close();
            in.close();
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
            objectStream.close();
            out.close();
            Log.i(LOG_TAG, "saving config file");
        } catch (IOException e) {
            e.printStackTrace();
            Log.e(LOG_TAG, "cannot save config file");
        }
    }

    private Drawable urlToDrawable(String url, Integer floor) throws IOException {
        Bitmap bmp;
        HttpURLConnection connection = (HttpURLConnection) new URL(url).openConnection();
        connection.connect();
        InputStream input = connection.getInputStream();
        bmp = BitmapFactory.decodeStream(input);
        copyStream(input, floor);
        return new BitmapDrawable(context.getResources(), bmp);
    }

    private void copyStream(InputStream in, Integer floor) throws IOException {
        FileOutputStream out = context.openFileOutput(MAP_FILE_PREFIX + floor.toString(), Context.MODE_PRIVATE);
        int read = 0;
        byte[] bytes = new byte[1024];
        while ((read = in.read(bytes)) != -1) {
            out.write(bytes, 0, read);
        }
    }

    private void scanForMapFiles() {

    }

}
