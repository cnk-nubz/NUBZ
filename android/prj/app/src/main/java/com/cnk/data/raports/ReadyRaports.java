package com.cnk.data.raports;

import android.util.Log;

import com.cnk.data.DataTranslator;
import com.cnk.data.FileHandler;
import com.cnk.database.DatabaseHelper;
import com.cnk.database.models.RaportFile;
import com.cnk.database.realm.RaportFileRealm;
import com.cnk.exceptions.DatabaseLoadException;
import com.cnk.notificators.Observable;

import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.concurrent.ConcurrentHashMap;

public class ReadyRaports extends Observable<ReadyRaports.RaportsUpdateAction> {
    public interface RaportsUpdateAction {
        void doOnUpdate(int raportsQueueSize);
    }

    private static final String LOG_TAG = "ReadyRaports";
    private static ReadyRaports instance;
    private ConcurrentHashMap<Raport, RaportId> readyRaports;
    private DatabaseHelper dbHelper;

    public ReadyRaports() {
        readyRaports = new ConcurrentHashMap<>();
    }

    public static ReadyRaports getInstance() {
        if (instance == null) {
            instance = new ReadyRaports();
        }
        return instance;
    }

    public void setDbHelper(DatabaseHelper dbHelper) {
        this.dbHelper = dbHelper;
    }

    public void loadDbData() throws DatabaseLoadException {
        try {
            loadReadyRaportsFromDb();
        } catch (Exception e) {
            e.printStackTrace();
            throw new DatabaseLoadException();
        }
    }

    private void loadReadyRaportsFromDb() {
        List<RaportFile> toLoad = dbHelper.getAllReadyRaports();
        for (RaportFile file : toLoad) {
            Raport loaded = null;
            try {
                loaded = DataTranslator.getRaportFromStream(FileHandler.getInstance()
                                                                       .getFile(file.getFileName()));
            } catch (Exception e) {
                e.printStackTrace();
                Log.e(LOG_TAG, "Unable to get raport");
            }
            if (loaded != null) {
                Log.i(LOG_TAG, loaded.toString());
                readyRaports.put(loaded, new RaportId(file.getServerId()));
            }
        }
    }

    // only uses files ready to send which are used by one thread - uploading raports
    public Map<Raport, Integer> getAllReadyRaports() {
        Set<Raport> keys = readyRaports.keySet();
        Map<Raport, Integer> result = new HashMap<>();
        for (Raport k : keys) {
            result.put(k, readyRaports.get(k).getId());
        }
        return result;
    }

    // doesn't modify files, only modifies database entries which are used by one thread - uploading raports
    public void setServerId(Raport raport, Integer serverId) {
        readyRaports.put(raport, new RaportId(serverId));
        dbHelper.changeRaportServerId(raport.getId(), serverId);
    }

    // only modifies database entries which are used by one thread - uploading raports
    public void markRaportAsSent(Raport raport) {
        raport.markAsSent();
        readyRaports.remove(raport);
        dbHelper.changeRaportState(raport.getId(), RaportFileRealm.SENT);
        notifyObservers(readyRaports.size());
    }

    public void addNewReadyRaport(Raport newRaport) {
        readyRaports.put(newRaport, new RaportId(null));
        notifyObservers(readyRaports.size());
    }

    public void notifyObservers(Integer newRaportsQueueSize) {
        List<RaportsUpdateAction> actions = new ArrayList<>();
        for (RaportsUpdateAction action : observers.values()) {
            actions.add(action);
        }

        for (RaportsUpdateAction action : actions) {
            action.doOnUpdate(newRaportsQueueSize);
        }
    }

    private class RaportId {
        private Integer id;
        public RaportId(Integer id) {
            this.id = id;
        }

        public void setId(Integer id) {
            this.id = id;
        }

        public Integer getId() {
            return id;
        }
    }
}
