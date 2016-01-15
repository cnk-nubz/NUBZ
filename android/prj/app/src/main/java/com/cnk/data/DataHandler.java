package com.cnk.data;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Environment;
import android.util.Log;

import com.cnk.database.DatabaseHelper;
import com.cnk.database.models.DetailLevelRes;
import com.cnk.database.models.Exhibit;
import com.cnk.database.models.MapTileInfo;
import com.cnk.database.models.RaportFile;
import com.cnk.database.models.Version;
import com.cnk.database.realm.RaportFileRealm;
import com.cnk.utilities.Consts;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Observable;
import java.util.concurrent.ConcurrentHashMap;

public class DataHandler extends Observable {
    public enum Item {
        FLOOR_0_MAP_CHANGED("Map changed 0"),
        FLOOR_0_MAP_CHANGING("Map changing 0"),
        FLOOR_1_MAP_CHANGED("Map changed 1"),
        FLOOR_1_MAP_CHANGING("Map changing 1"),
        BOTH_FLOORS_MAP_CHANGED("Map changed both"),
        BOTH_FLOORS_MAP_CHANGING("Map changing both"),
        MAP_UPDATE_COMPLETED("Map update completed"),
        EXPERIMENT_DATA("Experiment data"),
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
    private static final String FLOOR1_DIRECTORY = "floor0/";
    private static final String FLOOR2_DIRECTORY = "floor1/";
    private static final String FLOOR_DIRECTORY_PREFIX = "floor";
    private static final String TILE_FILE_PREFIX = "tile";
    private static final String RAPORT_FILE_PREFIX = "raport";
    private static final String TMP = "TMP";

    private final Map<Integer, String> cachedTileAdresses;

    private static DataHandler instance;
    private DatabaseHelper dbHelper;
    private Raport currentRaport;
    private ExperimentData experimentData;

    public static DataHandler getInstance() {
        if (instance == null) {
            instance = new DataHandler();
        }
        return instance;
    }

    public void setDbHelper(DatabaseHelper dbHelper) {
        this.dbHelper = dbHelper;
    }

    private DataHandler() {
        cachedTileAdresses = new ConcurrentHashMap<>();
    }

    public void setNewExperimentData(ExperimentData newData) {
        experimentData = newData;
        setChanged();
        notifyObservers(Item.EXPERIMENT_DATA);
    }

    public List<Action> getAllExhibitActions() {
        return experimentData.getExhibitActions();
    }
    public List<Action> getAllBreakActions() {
        return experimentData.getBreakActions();
    }

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

    public void setMaps(Integer version, FloorMap floor0, FloorMap floor1) throws IOException {
        Log.i(LOG_TAG, "Setting new maps");
        setChanged();

        Boolean floor0Changed = downloadAndSaveFloor(floor0, Consts.FLOOR1);
        Boolean floor1Changed = downloadAndSaveFloor(floor1, Consts.FLOOR2);

        if (floor0Changed && floor1Changed) {
            notifyObservers(Item.BOTH_FLOORS_MAP_CHANGING);
        } else if (floor0Changed) {
            notifyObservers(Item.FLOOR_0_MAP_CHANGING);
        } else if (floor1Changed) {
            notifyObservers(Item.FLOOR_1_MAP_CHANGING);
        }

        if (floor0Changed) {
            FileHandler.getInstance().renameFile(DATA_PATH + MAP_DIRECTORY + FLOOR_DIRECTORY_PREFIX
                    + TMP + Consts.FLOOR1.toString(), FLOOR1_DIRECTORY);
        }
        if (floor1Changed) {
            FileHandler.getInstance().renameFile(DATA_PATH + MAP_DIRECTORY + FLOOR_DIRECTORY_PREFIX
                            + TMP + Consts.FLOOR2.toString(), FLOOR2_DIRECTORY);
        }

        cachedTileAdresses.clear();

        Log.i(LOG_TAG, "Files saved, saving to db");
        dbHelper.setMaps(version, floor0, floor1);
        setChanged();

        if (floor0Changed && floor1Changed) {
            notifyObservers(Item.BOTH_FLOORS_MAP_CHANGED);
        } else if (floor0Changed) {
            notifyObservers(Item.FLOOR_0_MAP_CHANGED);
        } else if (floor1Changed) {
            notifyObservers(Item.FLOOR_1_MAP_CHANGED);
        }

        notifyMapUpdated();

        Log.i(LOG_TAG, "New maps set");
    }

    public void notifyMapUpdated() {
        setChanged();
        notifyObservers(Item.MAP_UPDATE_COMPLETED);
    }

    private Integer getTileCode(Integer floor, Integer detailLevel, Integer row, Integer column) {
        Integer code = 0;
        code += column;
        code += row * 1000;
        code += detailLevel * 1000 * 1000;
        code += floor * 10 * 1000 * 1000;

        return code;
    }

    public Bitmap getTile(Integer floor, Integer detailLevel, Integer row, Integer column) {
        String tileFilename = null;
        Integer tileCode = getTileCode(floor, detailLevel, row, column);

        tileFilename = cachedTileAdresses.get(tileCode);

        if (tileFilename == null) {
            tileFilename = dbHelper.getMapTileFileLocation(floor, detailLevel, row, column);

            cachedTileAdresses.put(tileCode, tileFilename);
        }

        Bitmap bmp = null;
        InputStream in = null;
        try {
            in = FileHandler.getInstance().getFile(tileFilename);
            bmp = BitmapFactory.decodeStream(in);
        } catch (FileNotFoundException e) {
            e.printStackTrace();
            Log.e(LOG_TAG, "File " + tileFilename + " not found");
        } finally {
            if (in != null) {
                try {
                    in.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
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

    public Resolution getTileSize(Integer floor, Integer detailLevel) {
        MapTileInfo size = dbHelper.getMapTileInfo(floor, detailLevel);
        return size != null ? size.getTileSize() : null;
    }

    public void setExhibits(List<Exhibit> exhibits, Integer version) {
        if (exhibits.isEmpty()) {
            return;
        }

        dbHelper.addOrUpdateExhibits(version, exhibits);
        setChanged();
        notifyObservers(Item.EXHIBITS);
    }

    public List<Exhibit> getExhibitsOfFloor(Integer floor) {
        return dbHelper.getAllExhibitsForFloor(floor);
    }

    public Exhibit getExhibit(Integer id) {
        return dbHelper.getExhibit(id);
    }

    public Integer getExhibitsVersion() {
        return dbHelper.getVersion(Version.Item.EXHIBITS);
    }

    public String getPathForTile(Integer floorNo, Integer detailLevel, Integer x, Integer y) {
        String dir = DATA_PATH + MAP_DIRECTORY +
                FLOOR_DIRECTORY_PREFIX + floorNo.toString() + "/"
                + detailLevel.toString() + "/";
        return dir + getTileFilename(x, y);
    }

    public String getTemporaryPathForTile(Integer floorNo, Integer detailLevel, Integer x, Integer y) {
        String dir = DATA_PATH + MAP_DIRECTORY +
                FLOOR_DIRECTORY_PREFIX + TMP + floorNo.toString() + "/" + detailLevel.toString() + "/";
        return dir + getTileFilename(x, y);
    }

    private String getTileFilename(Integer x, Integer y) {
        return TILE_FILE_PREFIX + x.toString() + "_" + y.toString();
    }

    private Boolean downloadAndSaveFloor(FloorMap floor, Integer floorNo) throws IOException {
        if (floor == null) {
            return false;
        }

        ArrayList<MapTiles> levels = floor.getLevels();
        for (int level = 0; level < levels.size(); level++) {
            ArrayList<ArrayList<String>> tiles = levels.get(level).getTilesFiles();
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
        return true;
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
