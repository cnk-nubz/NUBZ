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
    private static final String MAP_DIRECTORY = "maps";
    private static final String TILE_FILE_PREFIX = "tile";
    private static final String RAPORT_FILE_PREFIX = "raport";
    private static final String TMP = "TMP";
    private static final Integer MAX_TRIES = 3;

    private static DataHandler instance;
    private DatabaseHelper dbHelper;

    private List<List<DetailLevelRes>> detailLevelRes;
    private List<List<Exhibit>> exhibits;
    private List<List<MapTileInfo>> mapTilesSizes;
    private List<Integer> detailLevelsCount;
    private Map<Integer, Exhibit> exhibitIdMap;
    Integer exhibitsVersion;
    Integer mapVersion;
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
        detailLevelRes = new ArrayList<>();
        exhibits = new ArrayList<>();
        mapTilesSizes = new ArrayList<>();
        detailLevelsCount = new ArrayList<>();
        exhibitIdMap = new HashMap<>();
        mapVersion = null;
        exhibitsVersion = null;
    }

    public void loadDbData() {
        exhibitsVersion = dbHelper.getVersion(Version.Item.EXHIBITS);
        mapVersion = dbHelper.getVersion(Version.Item.MAP);
        for (int floor = 0; floor < Consts.FLOOR_COUNT; floor++) {
            exhibits.add(dbHelper.getAllExhibitsForFloor(floor));
            detailLevelsCount.add(dbHelper.getDetailLevelsForFloor(floor));
            detailLevelRes.add(new ArrayList<DetailLevelRes>());
            mapTilesSizes.add(new ArrayList<MapTileInfo>());
            for (int detailLevel = 0; detailLevel < detailLevelsCount.get(floor); detailLevel++) {
                detailLevelRes.get(floor).add(dbHelper.getDetailLevelRes(floor, detailLevel));
                mapTilesSizes.get(floor).add(dbHelper.getMapTileInfo(floor, detailLevel));
            }
        }

        for (int floor = 0; floor < Consts.FLOOR_COUNT; floor++) {
            for (Exhibit e : exhibits.get(floor)) {
                exhibitIdMap.put(e.getId(), e);
            }
        }
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
    public void startNewRaport() {
        Integer newId = dbHelper.getNextRaportId();
        currentRaport = new Raport(newId);
        String path = "";
        try {
            path = saveRaport(currentRaport);
        } catch (IOException e) {
            e.printStackTrace();
            Log.e(LOG_TAG, "IOException in startNewRaport");
            throw new RuntimeException("Cannot start new raport");
        }

        dbHelper.setRaportFile(newId, path);
    }

    // only modifies file of raport in progress, which is not used anywhere else at the time
    public void addEventToRaport(RaportEvent event, int tryNo) {
        currentRaport.addEvent(event);
        try {
            saveRaport(currentRaport);
        } catch (IOException e) {
            e.printStackTrace();
            Log.e(LOG_TAG, "IOException in addEventToRaport");
            if (tryNo > MAX_TRIES) {
                throw new RuntimeException("Cannot add event to raport");
            } else {
                addEventToRaport(event, tryNo + 1);
            }
        }

    }

    // only modifies database entry for raport in progress, marking it as ready, the raport cannot be used anywhere else
    public void markRaportAsReady(int tryNo) {
        try {
            saveRaport(currentRaport);
        } catch (IOException e) {
            e.printStackTrace();
            Log.e(LOG_TAG, "IOException in markRaportAsReady");
            if (tryNo > MAX_TRIES) {
                throw new RuntimeException("Cannot mark raport as ready");
            } else {
                markRaportAsReady(tryNo + 1);
                return;
            }
        }
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

    public void setMaps(Integer version, FloorMap floor0, FloorMap floor1, int tryNo) throws IOException {
        Log.i(LOG_TAG, "Setting new maps");
        Boolean floor0Changed = downloadAndSaveFloor(floor0, Consts.FLOOR1);
        Boolean floor1Changed = downloadAndSaveFloor(floor1, Consts.FLOOR2);
        if (floor0Changed || floor1Changed) {
            FileHandler.getInstance().renameFile(DATA_PATH + MAP_DIRECTORY + TMP, MAP_DIRECTORY);
        }
        Log.i(LOG_TAG, "Files saved, saving to db");
        dbHelper.setMaps(version, floor0, floor1);
        ArrayList<FloorMap> floors = new ArrayList<>();
        floors.add(floor0);
        floors.add(floor1);
        updateMapData(floors, version);
        setChanged();
        notifyMapUpdated();

        Log.i(LOG_TAG, "New maps set");
    }

    private void updateMapData(List<FloorMap> floors, Integer version) {
        mapVersion = version;
        detailLevelsCount.clear();
        detailLevelRes.clear();
        mapTilesSizes.clear();
        for (int floor = 0; floor < floors.size(); floor++) {
            FloorMap floorMap = floors.get(floor);
            detailLevelsCount.add(floorMap.getLevels().size());
            detailLevelRes.add(floor, new ArrayList<DetailLevelRes>());
            mapTilesSizes.add(floor, new ArrayList<MapTileInfo>());
            for (int i = 0; i < detailLevelsCount.get(floor); i++) {
                detailLevelRes.get(floor).add(new DetailLevelRes(
                                                                floor,
                                                                i,
                                                                floorMap.getOriginalSize(),
                                                                floorMap.getLevels().get(i).getScaledSize()
                                                                ));
                mapTilesSizes.get(floor).add(new MapTileInfo(
                                                            floor,
                                                            i,
                                                            floorMap.getLevels().get(i).getTileSize()
                                                            ));
            }
        }
    }

    public void notifyMapUpdated() {
        setChanged();
        notifyObservers(Item.MAP_UPDATE_COMPLETED);
    }

    public Bitmap getTile(Integer floor, Integer detailLevel, Integer row, Integer column) {
        String tileFilename = getTileFilename(row, column, floor, detailLevel);
        tileFilename = DATA_PATH + MAP_DIRECTORY + "/" + tileFilename;
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
        return detailLevelsCount.get(floor) > 0;
    }

    public Integer getDetailLevelsCountForFloor(Integer floor) {
        return detailLevelsCount.get(floor);
    }

    public Resolution getOriginalResolution(Integer floor) {
        return detailLevelRes.get(floor).get(0).getOriginalRes();
    }

    public Integer getMapVersion() {
        return dbHelper.getVersion(Version.Item.MAP);
    }

    public DetailLevelRes getDetailLevelResolution(Integer floor, Integer detailLevel) {
        return detailLevelRes.get(floor).get(detailLevel);
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
        exhibitsVersion = version;
        updateExhibits(exhibits);
        setChanged();
        notifyObservers(Item.EXHIBITS);
    }

    private void updateExhibits(List<Exhibit> newExhibits) {
        for (Exhibit newExhibit : newExhibits) {
            findAndChangeExhibit(newExhibit);
        }
    }

    private void findAndChangeExhibit(Exhibit e) {
        for (int floor = 0; floor < Consts.FLOOR_COUNT; floor++) {
            Exhibit old = exhibitIdMap.get(e.getId());
            if (old != null) {
                exhibits.get(floor).remove(old);
            }
        }
        if (e.getFloor() != null) {
            exhibits.get(e.getFloor()).add(e);
            exhibitIdMap.put(e.getId(), e);
        } else {
            exhibitIdMap.remove(e);
        }

    }

    public List<Exhibit> getExhibitsOfFloor(Integer floor) {
        return exhibits.get(floor);
    }

    public Integer getExhibitsVersion() {
        return exhibitsVersion;
    }

    public String getPathForTile(Integer floorNo, Integer detailLevel, Integer x, Integer y) {
        String dir = DATA_PATH + MAP_DIRECTORY;
        return dir + getTileFilename(x, y, floorNo, detailLevel);
    }

    public String getTemporaryPathForTile(Integer floorNo, Integer detailLevel, Integer x, Integer y) {
        String dir = DATA_PATH + MAP_DIRECTORY + TMP + "/";
        return dir + getTileFilename(x, y, floorNo, detailLevel);
    }

    private String getTileFilename(Integer x, Integer y, Integer floorNo, Integer detailLevel) {
        return TILE_FILE_PREFIX + floorNo.toString() + "_" + detailLevel.toString() + "_" + x.toString() + "_" + y.toString();
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
        String realFile = "";
        FileHandler.getInstance().saveSerializable(raport, tmpFile);
        realFile = RAPORT_FILE_PREFIX + raport.getId().toString();
        FileHandler.getInstance().renameFile(tmpFile, realFile);

        return dir + realFile;
    }
}
