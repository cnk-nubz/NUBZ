package com.cnk.data.raports;

import android.util.Log;

import com.cnk.data.DataTranslator;
import com.cnk.data.FileHandler;
import com.cnk.data.experiment.raport.Raport;
import com.cnk.database.DatabaseHelper;
import com.cnk.database.models.RaportFile;
import com.cnk.database.realm.RaportFileRealm;
import com.cnk.exceptions.DatabaseLoadException;

import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class ReadyRaports {
    private static final String LOG_TAG = "ReadyRaports";
    private static ReadyRaports instance;
    private Map<Raport, Integer> readyRaports;
    private DatabaseHelper dbHelper;

    public ReadyRaports() {
        readyRaports = Collections.synchronizedMap(new HashMap<>());
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
                loaded =
                        DataTranslator.getRaportFromStream(FileHandler.getInstance()
                                                                      .getFile(file.getFileName()));
            } catch (Exception e) {
                e.printStackTrace();
                Log.e(LOG_TAG, "Unable to get raport");
            }
            if (loaded != null) {
                Log.i(LOG_TAG, loaded.toString());
                readyRaports.put(loaded, file.getServerId());
            }
        }
    }

    // only uses files ready to send which are used by one thread - uploading raports
    public Map<Raport, Integer> getAllReadyRaports() {
        return readyRaports;
    }

    // doesn't modify files, only modifies database entries which are used by one thread - uploading raports
    public void setServerId(Raport raport, Integer serverId) {
        readyRaports.put(raport, serverId);
        dbHelper.changeRaportServerId(raport.getId(), serverId);
    }

    // only modifies database entries which are used by one thread - uploading raports
    public void markRaportAsSent(Raport raport) {
        readyRaports.remove(raport);
        dbHelper.changeRaportState(raport.getId(), RaportFileRealm.SENT);
    }

    public void addNewReadyRaport(Raport newRaport) {
        readyRaports.put(newRaport, null);
    }
}
