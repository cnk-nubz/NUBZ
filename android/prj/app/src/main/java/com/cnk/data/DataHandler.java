package com.cnk.data;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;
import android.util.Log;

import com.cnk.database.Exhibit;
import com.cnk.database.DatabaseHelper;
import com.cnk.database.Version;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.URL;
import java.util.List;
import java.util.Observable;

public class DataHandler extends Observable {
    public enum Item {
        MAP("Map"),
        EXHIBITS("Exhibits"),
        UNKNOWN("Unknown");

        private String code;

        private Item(String code) {
            this.code = code;
        }

        @Override
        public String toString() {
            return code;
        }

        public static Item fromString(String code) {
            if (code != null) {
                for (Item i : Item.values()) {
                    if (code.equals(i.code)) {
                        return i;
                    }
                }
            }
            return Item.UNKNOWN;
        }
    }

    private static final String LOG_TAG = "DataHandler";
    private static final String MAP_FILE_PREFIX = "map";
    private static final String PROTOCOL = "http://";
    private static DataHandler instance;
    private Context context;
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

    private DataHandler() {
    }

    public synchronized Drawable getFloorMap(Integer floor) {
        try {
            return getMapFromFile(dbHelper.getMapFile(floor));
        } catch (IOException e) {
            e.printStackTrace();
            return null;
        }
    }

    public synchronized void setMaps(Integer version, String urlFloor1, String urlFloor2) throws IOException {
        if (urlFloor1 != null) {
            downloadMap(urlFloor1, 0);
        }
        if (urlFloor2 != null) {
            downloadMap(urlFloor2, 1);
        }

        String floor1File = null;
        String floor2File = null;

        if (urlFloor1 != null) {
            floor1File = MAP_FILE_PREFIX + "0";
    }
        if (urlFloor2 != null) {
            floor2File = MAP_FILE_PREFIX + "1";
        }

        dbHelper.setMaps(version, floor1File, floor2File);

        setChanged();
        notifyObservers(Item.MAP);

    }

    public synchronized Integer getMapVersion() {
        if (dbHelper == null) {
            Log.e("DB HELPER", "IS NULL");
        }
        return dbHelper.getVersion(Version.Item.MAP);
    }

    public synchronized void setExhibits(List<Exhibit> exhibits, Integer version) {
        dbHelper.addOrUpdateExhibits(version, exhibits);
        setChanged();
        notifyObservers(Item.EXHIBITS);
    }

    public synchronized List<Exhibit> getExhibitsOfFloor(Integer floor) {
        return dbHelper.getAllExhibitsForFloor(floor);
    }

    public synchronized Exhibit getExhibit(Integer id) {
        return dbHelper.getExhibit(id);
    }

    public Integer getExhibitsVersion() {
        return dbHelper.getVersion(Version.Item.EXHIBITS);
    }

    private void downloadMap(String url, Integer floor) throws IOException {
        URL fileUrl = new URL(PROTOCOL + url);
        InputStream input = fileUrl.openStream();
        Bitmap bmp = BitmapFactory.decodeStream(input);
        input.close();
        saveMapFile(bmp, floor);
    }

    private void saveMapFile(Bitmap bmp, Integer floor) throws IOException {
        try {
            FileOutputStream out = context.openFileOutput(MAP_FILE_PREFIX + floor.toString(), Context.MODE_PRIVATE);
            bmp.compress(Bitmap.CompressFormat.JPEG, 100, out);
            out.close();
            Log.i(LOG_TAG, "Map saved to file " + MAP_FILE_PREFIX + floor.toString());
        } catch (IOException e) {
            e.printStackTrace();
            Log.e(LOG_TAG, "Saving map file failed");
            throw e;
        }
    }

    private Drawable getMapFromFile(String file) throws IOException {
        if (file == null) {
            return null;
        }
        try {
            FileInputStream in = context.openFileInput(file);
            BitmapFactory.Options options = new BitmapFactory.Options();
            options.inPreferredConfig = Bitmap.Config.RGB_565;
            Bitmap bmp = BitmapFactory.decodeStream(in, null, options);
            return new BitmapDrawable(context.getResources(), bmp);
        } catch (FileNotFoundException e) {
            e.printStackTrace();
            throw e;
        }
    }
}
