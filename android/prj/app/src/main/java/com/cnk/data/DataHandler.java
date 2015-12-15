package com.cnk.data;

import android.graphics.Bitmap;
import android.util.Log;

import com.cnk.database.DatabaseHelper;
import com.cnk.database.models.DetailLevelRes;
import com.cnk.database.models.Exhibit;
import com.cnk.database.models.RaportFile;
import com.cnk.database.models.Version;
import com.cnk.database.realm.RaportFileRealm;

import java.io.IOException;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Observable;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

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
    private static DataHandler instance;
    private DatabaseHelper dbHelper;
    private Raport currentRaport;
    private Lock mapDBLock;

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
        mapDBLock = new ReentrantLock(true);
    }

    // only creates new database entry and file for new raport which is not used anywhere else
    public void startNewRaport() throws IOException {
        Integer newId = dbHelper.getNextRaportId();
        currentRaport = new Raport(newId);
        String fileName = FileHandler.getInstance().saveRaportToFile(currentRaport);
        dbHelper.setRaportFile(newId, fileName);
    }

    // only modifies file of raport in progress, which is not used anywhere else at the time
    public void addEventToRaport(RaportEvent event) throws IOException {
        currentRaport.addEvent(event);
        FileHandler.getInstance().saveRaportToFile(currentRaport);
    }

    // only modifies database entry for raport in progress, marking it as ready, the raport cannot be used anywhere else
    public void markRaportAsReady() {
        dbHelper.changeRaportState(currentRaport.getId(), RaportFileRealm.READY_TO_SEND);
        currentRaport = null;
    }

    // only modifies files ready to send which are used by one thread - uploading raports
    public Map<Raport, Integer> getAllReadyRaports() {
        List<RaportFile> toLoad = dbHelper.getAllReadyRaports();
        Map<Raport, Integer> raportsToSend = new HashMap<>();
        for (RaportFile file : toLoad) {
            Raport loaded = FileHandler.getInstance().getRaport(file.getFileName());
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
        FileHandler.getInstance().downloadAndSaveMaps(floor1, floor2);
        Log.i(LOG_TAG, "Files saved, saving to db");
        mapDBLock.lock();
        dbHelper.setMaps(version, floor1, floor2);
        mapDBLock.unlock();
        setChanged();
        notifyObservers(Item.MAP_CHANGED);
        Log.i(LOG_TAG, "New maps set");
    }

    public Bitmap getTile(Integer floor, Integer detailLevel, Integer row, Integer column) {
        mapDBLock.lock();
        String tileFilename = dbHelper.getMapTileFileLocation(floor, detailLevel, row, column);
        mapDBLock.unlock();
        return FileHandler.getInstance().getTileBitmap(tileFilename);
    }


    public Boolean mapForFloorExists(Integer floor) {
        mapDBLock.lock();
        boolean exists = dbHelper.getDetailLevelsForFloor(floor) != null;
        mapDBLock.unlock();
        return exists;
    }

    public Integer getDetailLevelsCountForFloor(Integer floor) {
        mapDBLock.lock();
        Integer detailLevels = dbHelper.getDetailLevelsForFloor(floor);
        mapDBLock.unlock();
        return detailLevels;
    }

    public Resolution getOriginalResolution(Integer floor) {
        mapDBLock.lock();
        Resolution originalRes = dbHelper.getDetailLevelRes(floor, 1).getOriginalRes();
        mapDBLock.unlock();
        return originalRes;
    }


    public Integer getMapVersion() {
        mapDBLock.lock();
        Integer version = dbHelper.getVersion(Version.Item.MAP);
        mapDBLock.unlock();
        return version;
    }

    public DetailLevelRes getDetailLevelResolution(Integer floor, Integer detailLevel) {
        mapDBLock.lock();
        DetailLevelRes res = dbHelper.getDetailLevelRes(floor, detailLevel);
        mapDBLock.unlock();
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
}
