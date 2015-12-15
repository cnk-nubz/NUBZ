package com.cnk.data;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Environment;
import android.util.Log;

import com.cnk.database.DatabaseHelper;
import com.cnk.database.models.DetailLevelRes;
import com.cnk.database.models.Exhibit;
import com.cnk.database.models.RaportFile;
import com.cnk.database.models.Version;
import com.cnk.database.realm.RaportFileRealm;
import com.cnk.utilities.Consts;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Observable;

public class DataHandler extends Observable {
    public enum Item {
        MAP_CHANGED("Map changed"),
        MAP_CHANGING("Map changing"),
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
    private static final String DATA_PATH = Environment.getExternalStorageDirectory() + "/nubz/";
    private static final String RAPORT_DIRECTORY = "raports/";
    private static final String MAP_DIRECTORY = "maps/";
    private static final String FLOOR1_DIRECTORY = "floor1/";
    private static final String FLOOR2_DIRECTORY = "floor2/";
    private static final String TILE_FILE_PREFIX = "tile";
    private static final String RAPORT_FILE_PREFIX = "raport";
    private static final String TMP = "TMP";

    private static DataHandler instance;
    private DatabaseHelper dbHelper;
    private Raport currentRaport;

    public static DataHandler getInstance() {
        if (instance == null) {
            instance = new DataHandler();
        }
        return instance;
    }

    public void setDbHelper(DatabaseHelper dbHelper) {
        this.dbHelper = dbHelper;
    }

    private DataHandler() {}

    // only creates new database entry and file for new raport which is not used anywhere else
    public void startNewRaport() throws IOException {
        Integer newId = dbHelper.getNextRaportId();
        currentRaport = new Raport(newId);
        String path = saveRaport(currentRaport);
        dbHelper.setRaportFile(newId, path);
    }

    // only modifies file of raport in progress, which is not used anywhere else at the time
    public void addEventToRaport(RaportEvent event) throws IOException {
        currentRaport.addEvent(event);
        saveRaport(currentRaport);
    }

    // only modifies database entry for raport in progress, marking it as ready, the raport cannot be used anywhere else
    public void markRaportAsReady() throws IOException {
        saveRaport(currentRaport);
        dbHelper.changeRaportState(currentRaport.getId(), RaportFileRealm.READY_TO_SEND);
        currentRaport = null;
    }

    // only modifies files ready to send which are used by one thread - uploading raports
    public Map<Raport, Integer> getAllReadyRaports() {
        List<RaportFile> toLoad = dbHelper.getAllReadyRaports();
        Map<Raport, Integer> raportsToSend = new HashMap<>();
        for (RaportFile file : toLoad) {
            Raport loaded = null;
            try {
                loaded = DataTranslator.getRaportFromStream(FileHandler.getInstance().getFile(file.getFileName()));
            } catch (Exception e) {
                e.printStackTrace();
                Log.e(LOG_TAG, "Unable to get raport");
            }
            if (loaded != null) {
                raportsToSend.put(loaded, file.getServerId());
            }
        }
        return raportsToSend;
    }

    // doesn't modify files, only modifies database entries which are used by one thread - uploading raports
    public void setServerId(Raport raport, Integer serverId) {
        dbHelper.changeRaportServerId(raport.getId(), serverId);
    }

    // only modifies database entries which are used by one thread - uploading raports
    public void markRaportAsSent(Raport raport) {
        dbHelper.changeRaportState(raport.getId(), RaportFileRealm.SENT);
    }

    public void setMaps(Integer version, FloorMap floor1, FloorMap floor2) throws IOException {
        Log.i(LOG_TAG, "Setting new maps");
        setChanged();
        notifyObservers(Item.MAP_CHANGING);
        downloadAndSaveFloor(floor1, Consts.FLOOR1);
        downloadAndSaveFloor(floor2, Consts.FLOOR2);
        FileHandler.getInstance().renameFile(DATA_PATH + MAP_DIRECTORY + TMP + Consts.FLOOR1.toString(),
                FLOOR1_DIRECTORY);
        FileHandler.getInstance().renameFile(DATA_PATH + MAP_DIRECTORY + TMP + Consts.FLOOR2.toString(),
                FLOOR2_DIRECTORY);
        Log.i(LOG_TAG, "Files saved, saving to db");
        dbHelper.setMaps(version, floor1, floor2);
        setChanged();
        notifyObservers(Item.MAP_CHANGED);
        Log.i(LOG_TAG, "New maps set");
    }

    public Bitmap getTile(Integer floor, Integer detailLevel, Integer row, Integer column) {
        String tileFilename = dbHelper.getMapTileFileLocation(floor, detailLevel, row, column);
        Bitmap bmp = null;
        try {
            bmp = BitmapFactory.decodeStream(FileHandler.getInstance().getFile(tileFilename));
        } catch (FileNotFoundException e) {
            e.printStackTrace();
            Log.e(LOG_TAG, "File " + tileFilename + " not found");
        }
        return bmp;
    }


    public Boolean mapForFloorExists(Integer floor) {
        boolean exists = dbHelper.getDetailLevelsForFloor(floor) != null;
        return exists;
    }

    public Integer getDetailLevelsCountForFloor(Integer floor) {
        Integer detailLevels = dbHelper.getDetailLevelsForFloor(floor);
        return detailLevels;
    }

    public Resolution getOriginalResolution(Integer floor) {
        Resolution originalRes = dbHelper.getDetailLevelRes(floor, 1).getOriginalRes();
        return originalRes;
    }


    public Integer getMapVersion() {
        Integer version = dbHelper.getVersion(Version.Item.MAP);
        return version;
    }

    public DetailLevelRes getDetailLevelResolution(Integer floor, Integer detailLevel) {
        DetailLevelRes res = dbHelper.getDetailLevelRes(floor, detailLevel);
        return res;
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

    public String getPathForTile(Integer floorNo, Integer detailLevel, Integer x, Integer y) {
        String dir = DATA_PATH + MAP_DIRECTORY +
                (floorNo.equals(Consts.FLOOR1) ? FLOOR1_DIRECTORY : FLOOR2_DIRECTORY)
                + detailLevel.toString();
        return dir + getTileFilename(x, y);
    }

    public String getTemporaryPathForTile(Integer floorNo, Integer detailLevel, Integer x, Integer y) {
        String dir = DATA_PATH + MAP_DIRECTORY + TMP + floorNo.toString() + "/" + detailLevel.toString() + "/";
        return dir + getTileFilename(x, y);
    }

    private String getTileFilename(Integer x, Integer y) {
        return "tile" + x.toString() + "_" + y.toString();
    }

    private void downloadAndSaveFloor(FloorMap floor, Integer floorNo) throws IOException {
        List<MapTiles> levels = floor.getLevels();
        for (int level = 0; level < levels.size(); level++) {
            List<List<String>> tiles = levels.get(level).getTilesFiles();
            for (int i = 0; i < tiles.size(); i++) {
                for (int j = 0; j < tiles.get(i).size(); j++) {
                    InputStream in = Downloader.getInstance().download(tiles.get(i).get(j));
                    String tmpFilename = getTemporaryPathForTile(floorNo, level, i, j);
                    String actualFilename = getPathForTile(floorNo, level, i, j);
                    FileHandler.getInstance().saveInputStream(in, tmpFilename);
                    tiles.get(i).set(j, actualFilename);
                }
            }
        }
    }

    private String saveRaport(Raport raport) throws IOException {
        String dir = DATA_PATH + RAPORT_DIRECTORY;
        new File(dir).mkdirs();
        String tmpFile = dir + TMP;
        FileHandler.getInstance().saveSerializable(raport, tmpFile);
        String realFile = RAPORT_FILE_PREFIX + raport.getId().toString();
        FileHandler.getInstance().renameFile(tmpFile, realFile);
        return dir + realFile;
    }
}
