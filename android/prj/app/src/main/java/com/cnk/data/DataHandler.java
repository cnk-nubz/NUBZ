package com.cnk.data;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;
import android.util.Log;

import com.cnk.database.DatabaseHelper;
import com.cnk.database.Version;
import com.cnk.exceptions.DatabaseException;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.URL;
import java.util.Map;
import java.util.Observable;
import java.util.concurrent.ConcurrentHashMap;

public class DataHandler extends Observable {

    private static final String LOG_TAG = "DataHandler";
    private static final String CONFIG_FILE = "config";
    private static final String MAP_FILE_PREFIX = "map";
    private static final String PROTOCOL = "http://";
    private static final Integer FLOORS = 2;
    private static DataHandler instance;
    private Context context;
    private Map<Integer, Drawable> mapDrawables;
    private DatabaseHelper dbHelper;

    public static DataHandler getInstance() {
        if (instance == null) {
            instance = new DataHandler();
        }
        return instance;
    }

    public void setContext(Context c) {
        context = c;
    }

    public void setDbHelper(DatabaseHelper dbHelper) {
        this.dbHelper = dbHelper;
    }

    public synchronized void getInitData() {
        for (int i = 0; i < FLOORS; i++) {
            try {
                String file = dbHelper.getMapFile(i);
                mapDrawables.put(i, getMapFromFile(file));
            } catch (IOException e){
                e.printStackTrace();
                Log.e(LOG_TAG, "Cannot get map file of floor " + Integer.toString(i));
            } catch (DatabaseException e2) {
                e2.printStackTrace();
                Log.e(LOG_TAG, "No record of floor " + Integer.toString(i));
            }
        }
    }

    private DataHandler() {
        mapDrawables = new ConcurrentHashMap<>();
    }

    public synchronized Drawable getFloorMap(Integer floor) {
        return mapDrawables.get(floor);
    }

    public synchronized void setMapFloor(Integer floor, String url) throws IOException {
        Drawable image = urlToDrawable(url, floor);
        mapDrawables.put(floor, image);
        dbHelper.setMapFile(floor, MAP_FILE_PREFIX + floor.toString());

        setChanged();
        notifyObservers();
    }

    public synchronized void setMapVersion(int newVersion) {
        dbHelper.setVersion(Version.Item.MAP, newVersion);
    }

    public synchronized Integer getMapVersion() {
        Integer version =  dbHelper.getVersion(Version.Item.MAP);
        return version;
    }

    private Drawable urlToDrawable(String url, Integer floor) throws IOException {
        Bitmap bmp;
        URL fileUrl = new URL(PROTOCOL + url);
        InputStream input = fileUrl.openStream();
        bmp = BitmapFactory.decodeStream(input);
        input.close();
        saveMapFile(bmp, floor);
        return new BitmapDrawable(context.getResources(), bmp);
    }

    private void saveMapFile(Bitmap bmp, Integer floor) throws IOException {
        try {
            FileOutputStream out = context.openFileOutput(MAP_FILE_PREFIX + floor.toString(), Context.MODE_PRIVATE);
            bmp.compress(Bitmap.CompressFormat.PNG, 100, out);
            out.close();
            Log.i(LOG_TAG, "Map saved to file " + MAP_FILE_PREFIX + floor.toString());
        } catch (IOException e) {
            e.printStackTrace();
            Log.e(LOG_TAG, "Saving map file failed");
            throw e;
        }
    }

    private Drawable getMapFromFile(String file) throws IOException {
        try {
            FileInputStream in = context.openFileInput(file);
            Bitmap bmp = BitmapFactory.decodeStream(in);
            return new BitmapDrawable(context.getResources(), bmp);
        } catch (FileNotFoundException e) {
            e.printStackTrace();
            throw new FileNotFoundException();
        }
    }
}
