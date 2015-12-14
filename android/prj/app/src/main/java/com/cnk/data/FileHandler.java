package com.cnk.data;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Environment;
import android.util.Log;

import com.cnk.utilities.Consts;

import org.apache.commons.io.FileUtils;
import org.apache.commons.io.IOUtils;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.URL;
import java.util.List;

public class FileHandler {

    private static final String DATA_PATH = Environment.getExternalStorageDirectory() + "/nubz/";
    private static final String RAPORT_DIRECTORY = "raports/";
    private static final String MAP_DIRECTORY = "maps/";
    private static final String FLOOR1_DIRECTORY = "floor1/";
    private static final String FLOOR2_DIRECTORY = "floor2/";
    private static final String TILE_FILE_PREFIX = "tile";
    private static final String RAPORT_FILE_PREFIX = "raport";
    private static final String TMP = "TMP";
    private static final String LOG_TAG = "FILE_HANDLER";
    private static final String PROTOCOL = "http://";

    private static FileHandler instance;

    public static FileHandler getInstance() {
        if (instance == null) {
            instance = new FileHandler();
        }
        return instance;
    }

    private FileHandler() {}

    public String getAddressofTile(Integer detailLevel, Integer floor, Integer x, Integer y) {
        String floorDirectory = floor.equals(Consts.FLOOR1) ? FLOOR1_DIRECTORY : FLOOR2_DIRECTORY;
        return DATA_PATH + MAP_DIRECTORY + floorDirectory + getTileFileName(x, y);
    }

    /*
    Downloads and saves tiles for floors, if the floor1 or floor2 object is null it does nothing for that floor
    Files are saved to temporary directory and renamed to matching name once the download is successful
     */
    public void downloadAndSaveMaps(FloorMap floor1, FloorMap floor2) throws IOException {
        File mapsDir = new File(DATA_PATH + MAP_DIRECTORY);
        mapsDir.mkdirs();
        File tmpFloor1File = null;
        File tmpFloor2File = null;
        try {
            tmpFloor1File = downloadAndSaveFloor(floor1, mapsDir, Consts.FLOOR1);
            tmpFloor2File = downloadAndSaveFloor(floor2, mapsDir, Consts.FLOOR2);
        } catch (IOException e) {
            if (tmpFloor1File != null && tmpFloor1File.exists()) {
                tmpFloor1File.delete();
            }
            if (tmpFloor2File != null && tmpFloor2File.exists()) {
                tmpFloor2File.delete();
            }
            throw e;
        }
        renameFile(tmpFloor1File, FLOOR1_DIRECTORY);
        renameFile(tmpFloor2File, FLOOR2_DIRECTORY);
    }

    public String saveRaportToFile(Raport raport) throws IOException {
        File dir = new File(DATA_PATH + RAPORT_DIRECTORY);
        dir.mkdirs();
        File tmpFile = new File(dir, TMP);
        try {
            FileOutputStream out = new FileOutputStream(tmpFile);
            ObjectOutputStream objectOut = new ObjectOutputStream(out);
            objectOut.writeObject(raport);
            objectOut.close();
            out.close();
        } catch (Exception e) {
            e.printStackTrace();
            throw new RuntimeException();
        }
        renameFile(tmpFile, RAPORT_FILE_PREFIX + raport.getId().toString());
        return RAPORT_FILE_PREFIX + raport.getId().toString();
    }

    public Raport getRaport(String filename) {
        FileInputStream in = null;
        ObjectInputStream objectIn = null;
        File dir = new File(DATA_PATH + RAPORT_DIRECTORY);
        Raport curRaport;
        try {
            in = new FileInputStream(dir.toString() + "/" + filename);
            objectIn = new ObjectInputStream(in);
            curRaport = (Raport) objectIn.readObject();
        } catch (Exception e) {
            e.printStackTrace();
            return null;
        } finally {
            closeInputStreams(in, objectIn);
        }
        return curRaport;
    }

    private void closeInputStreams(InputStream in, ObjectInputStream objectIn) {
        try {
            if (objectIn != null) {
                objectIn.close();
            }
            if (in != null) {
                in.close();
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private void renameFile(File from, String to) throws IOException {
        if (from == null) {
            return;
        }
        File dir = from.getParentFile();
        File newFile = new File(dir, to);
        Log.i(LOG_TAG, "Renaming " + from.toString() + " to " + to);
        if (newFile.exists()) {
            if (newFile.isDirectory()) {
                FileUtils.deleteDirectory(newFile);
            } else {
                newFile.delete();
            }
        }
        boolean renamed = from.renameTo(newFile);
        Log.i(LOG_TAG, "Renaming successful " + Boolean.toString(renamed));
    }

    /*
    Downloads and saves tile for one particualr floor, if the floor is null, nothing is done
    Saves files to temporary directory
     */
    private File downloadAndSaveFloor(FloorMap floor, File dir, Integer floorNo) throws IOException {
        if (floor == null) {
            return null;
        }
        Log.i(LOG_TAG, "Starting download and save of tiles for floor " + floorNo.toString());
        String floorDirName = floorNo.equals(Consts.FLOOR1) ? TMP + "1" : TMP + "2";
        Log.i(LOG_TAG, "Creating temporary directory " + floorDirName);
        File floorDir = new File(dir, floorDirName);
        floorDir.mkdirs();
        for (int i = 1; i < floor.getLevels().size() + 1; i++) {
            MapTiles currentLevel = floor.getLevels().get(i - 1);
            List<List<String>> urls = currentLevel.getTilesUrls();
            downloadAndSaveLevel(i, floorDir, urls);
        }
        Log.i(LOG_TAG, "Tiles for floor " + floorNo.toString() + " done");
        return floorDir;
    }

    private void downloadAndSaveLevel(Integer level, File dir, List<List<String>> urls) throws IOException {
        Log.i(LOG_TAG, "Downloading and saving level " + level.toString());
        File levelDir = new File(dir, level.toString());
        levelDir.mkdirs();
        for (int i = 0; i < urls.size(); i++) {
            for (int j = 0; j < urls.get(i).size(); j++) {
                String url = urls.get(i).get(j);
                downloadAndSaveTile(url, levelDir, i, j);
            }
        }
        Log.i(LOG_TAG, "Level " + level.toString() + " downloaded and saved");
    }

    public void downloadAndSaveTile(String url, File dir, Integer x, Integer y) throws IOException {
        URL fileUrl = new URL(PROTOCOL + url);
        InputStream in = fileUrl.openStream();
        File tileFile = new File(dir, getTileFileName(x, y));
        FileOutputStream out = new FileOutputStream(tileFile);
        IOUtils.copy(in, out);
        out.close();
        in.close();
        Log.i(LOG_TAG, "Tile " + x.toString() + " " + y.toString() + " saved");
    }

    /*
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
    */

    private String getTileFileName(Integer x, Integer y) {
        return TILE_FILE_PREFIX + x.toString() + "_" + y.toString();
    }

}
