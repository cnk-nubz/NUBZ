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

public class DataHandler extends Observable {
    public enum Item {
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

    private List<FloorInfo> floorInfos;
    Integer exhibitsVersion;
    Integer mapVersion;
    private Raport currentRaport;
    private Experiment experiment;

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
        floorInfos = new ArrayList<>();
        mapVersion = null;
        exhibitsVersion = null;
    }

    public void loadDbData() {
        exhibitsVersion = dbHelper.getVersion(Version.Item.EXHIBITS);
        mapVersion = dbHelper.getVersion(Version.Item.MAP);
        floorInfos.clear();
        for (int floor = 0; floor < Consts.FLOOR_COUNT; floor++) {
            FloorInfo currentFloor = new FloorInfo();
            currentFloor.setExhibits(exhibitListToMap(dbHelper.getAllExhibitsForFloor(floor)));
            currentFloor.setDetailLevelsCount(dbHelper.getDetailLevelsForFloor(floor));
            ArrayList<DetailLevelRes> detailLevelRes = new ArrayList<>();
            ArrayList<MapTileInfo> mapTileInfos = new ArrayList<>();
            for (int detailLevel = 0; detailLevel < currentFloor.getDetailLevelsCount(); detailLevel++) {
                detailLevelRes.add(dbHelper.getDetailLevelRes(floor, detailLevel));
                mapTileInfos.add(dbHelper.getMapTileInfo(floor, detailLevel));
            }
            currentFloor.setDetailLevelRes(detailLevelRes);
            currentFloor.setMapTilesSizes(mapTileInfos);
            floorInfos.add(currentFloor);
        }
    }

    private Map<Integer, Exhibit> exhibitListToMap(List<Exhibit> exhibitList) {
        Map<Integer, Exhibit> map = new HashMap<>();
        for (Exhibit e : exhibitList) {
            map.put(e.getId(), e);
        }
        return map;
    }

    public void setNewExperimentData(Experiment newData) {
        experiment = newData;
        setChanged();
        notifyObservers(Item.EXPERIMENT_DATA);
    }

    public List<Action> getAllExhibitActions() {
        return experiment.getExhibitActions();
    }
    public List<Action> getAllBreakActions() {
        return experiment.getBreakActions();
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
        if (tryNo == 0) {
            currentRaport.addEvent(event);
        }
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
        dbHelper.changeRaportState(currentRaport.getId(), RaportFileRealm.READY_TO_SEND);
        currentRaport = null;
    }

    // only uses files ready to send which are used by one thread - uploading raports
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
        loadDbData();
        setChanged();
        notifyMapUpdated();

        Log.i(LOG_TAG, "New maps set");
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
        return floorInfos.get(floor).getDetailLevelsCount() > 0;
    }

    public Integer getDetailLevelsCountForFloor(Integer floor) {
        return floorInfos.get(floor).getDetailLevelsCount();
    }

    public Resolution getOriginalResolution(Integer floor) {
        return floorInfos.get(floor).getDetailLevelRes().get(0).getOriginalRes();
    }

    public Integer getMapVersion() {
        return mapVersion;
    }

    public DetailLevelRes getDetailLevelResolution(Integer floor, Integer detailLevel) {
        return floorInfos.get(floor).getDetailLevelRes().get(detailLevel);
    }

    public Resolution getTileSize(Integer floor, Integer detailLevel) {
        MapTileInfo size = floorInfos.get(floor).getMapTilesSizes().get(detailLevel);
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
            floorInfos.get(floor).removeExhibit(e.getId());
        }
        if (e.getFloor() != null) {
            floorInfos.get(e.getFloor()).addExhibit(e);
        }
    }

    public List<Exhibit> getExhibitsOfFloor(Integer floor) {
        return floorInfos.get(floor).getExhibitsList();
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
