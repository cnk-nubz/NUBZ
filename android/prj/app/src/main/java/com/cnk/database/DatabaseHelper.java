package com.cnk.database;

import android.content.Context;
import android.util.Log;

import com.cnk.exceptions.NoSuchRecordException;

import io.realm.Realm;
import io.realm.RealmResults;

public class DatabaseHelper {
    Context applicationContext;
    Realm realm;

    public DatabaseHelper(Context applicationContext) {
        this.applicationContext = applicationContext;
    }

    public void open() {
        realm = Realm.getInstance(applicationContext);
    }

    public void close() {
        realm.close();
    }

    public Version getVersion(String item) throws NoSuchRecordException {
        RealmResults<Version> results = realm.where(Version.class)
                .equalTo("item", item).findAll();

        if (!results.isEmpty()) {
            return results.first();
        } else {
            throw new NoSuchRecordException();
        }
    }

    public void setVersion(String item, int currentVersion) throws RuntimeException {
        Version version = new Version();
        version.setItem(item);
        version.setCurrentVersion(currentVersion);

        try {
            realm.beginTransaction();
            realm.copyToRealmOrUpdate(version);
            realm.commitTransaction();
        } catch (Exception e) {
            Log.e("Realm Error", "error msg: " + e.toString());
            realm.cancelTransaction();
            throw e;
        }
    }

    public void removeItemVersion(String item) throws RuntimeException {
        Version version = getVersion(item);
        if (version != null) {
            try {
                realm.beginTransaction();
                version.removeFromRealm();
                realm.commitTransaction();
            } catch (Exception e) {
                Log.e("Realm Error", "error msg: " + e.toString());
                realm.cancelTransaction();
                throw e;
            }
        }
    }

    public void addAllVersions(Iterable<Version> items) throws RuntimeException {
        try {
            realm.beginTransaction();
            realm.copyToRealmOrUpdate(items);
            realm.commitTransaction();
        } catch (Exception e) {
            Log.e("Realm Error", "error msg: " + e.toString());
            realm.cancelTransaction();
            throw e;
        }
    }

    public MapFile getMapFile(int floor) throws NoSuchRecordException {
        RealmResults<MapFile> results = realm.where(MapFile.class)
                .equalTo("floor", floor).findAll();

        if (!results.isEmpty()) {
            return results.first();
        } else {
            throw new NoSuchRecordException();
        }
    }

    public void setMapFile(int floor, String mapFileLocation) throws RuntimeException {
        MapFile mapFile = new MapFile();
        mapFile.setFloor(floor);
        mapFile.setMapFileLocation(mapFileLocation);

        try {
            realm.beginTransaction();
            realm.copyToRealmOrUpdate(mapFile);
            realm.commitTransaction();
        } catch (Exception e) {
            Log.e("Realm Error", "error msg: " + e.toString());
            realm.cancelTransaction();
            throw e;
        }
    }
}
