package com.cnk.data;

import android.util.Log;

import org.apache.commons.io.FileUtils;
import org.apache.commons.io.IOUtils;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.ObjectOutputStream;
import java.io.Serializable;

public class FileHandler {

    private static final String LOG_TAG = "FILE_HANDLER";

    private static FileHandler instance;

    public static FileHandler getInstance() {
        if (instance == null) {
            instance = new FileHandler();
        }
        return instance;
    }

    private FileHandler() {}

    public void saveInputStream(InputStream in, String filename) throws IOException {
        File file = new File(filename);
        file.getParentFile().mkdirs();
        FileOutputStream out = new FileOutputStream(file);
        IOUtils.copy(in, out);
        out.close();
        in.close();
    }

    public void saveSerializable(Serializable toSave, String filename) throws IOException {
        File file = new File(filename);
        file.getParentFile().mkdirs();
        FileOutputStream out = new FileOutputStream(file);
        ObjectOutputStream objOut = new ObjectOutputStream(out);
        objOut.writeObject(toSave);
        out.close();
        objOut.close();
    }

    public FileInputStream getFile(String filename) throws FileNotFoundException {
        File file = new File(filename);
        return new FileInputStream(file);
    }

    public void renameFile(String from, String to) throws IOException {
        if (from == null) {
            return;
        }
        File file = new File(from);
        File dir = file.getParentFile();
        File newFile = new File(dir, to);
        Log.i(LOG_TAG, "Renaming " + from + " to " + to);
        if (newFile.exists()) {
            if (newFile.isDirectory()) {
                FileUtils.deleteDirectory(newFile);
            } else {
                newFile.delete();
            }
        }
        file.renameTo(newFile);
    }

    /*
    Downloads and saves tiles for floors, if the floor1 or floor2 object is null it does nothing for that floor
    Files are saved to temporary directory and renamed to matching name once the download is successful

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
    public Bitmap getTileBitmap(String filename) {
        try {
            FileInputStream in = new FileInputStream(filename);
            Bitmap bmp = BitmapFactory.decodeStream(in);
            in.close();
            return bmp;
        } catch (Exception e) {
            e.printStackTrace();
            return null;
        }
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

    /*
    Downloads and saves tile for one particualr floor, if the floor is null, nothing is done
    Saves files to temporary directory

    private File downloadAndSaveFloor(FloorMap floor, File dir, Integer floorNo) throws IOException {
        if (floor == null) {
            return null;
        }
        Log.i(LOG_TAG, "Starting download and save of tiles for floor " + floorNo.toString());
        String floorDirName = floorNo.equals(Consts.FLOOR1) ? TMP + "1" : TMP + "2";
        Log.i(LOG_TAG, "Creating temporary directory " + floorDirName);
        File floorDir = new File(dir, floorDirName);
        if (floorDir.exists()) {
            FileUtils.deleteDirectory(floorDir);
        }
        floorDir.mkdirs();
        for (int i = 1; i < floor.getLevels().size() + 1; i++) {
            MapTiles currentLevel = floor.getLevels().get(i - 1);
            List<List<String>> urls = currentLevel.getTilesFiles();
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
                String tileFile = downloadAndSaveTile(url, levelDir, i, j);
                urls.get(i).set(j, tileFile);
            }
        }
        Log.i(LOG_TAG, "Level " + level.toString() + " downloaded and saved");
    }

    private String downloadAndSaveTile(String url, File dir, Integer x, Integer y) throws IOException {
        URL fileUrl = new URL(PROTOCOL + url);
        InputStream in = fileUrl.openStream();
        File tileFile = new File(dir, getTileFileName(x, y));
        FileOutputStream out = new FileOutputStream(tileFile);
        IOUtils.copy(in, out);
        out.close();
        in.close();
        //Log.i(LOG_TAG, "Tile " + x.toString() + " " + y.toString() + " saved");
        return tileFile.toString();
    }

    private String getTileFileName(Integer x, Integer y) {
        return TILE_FILE_PREFIX + x.toString() + "_" + y.toString();
    }
    */

}
