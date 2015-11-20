package com.cnk.database;

import android.content.Context;
import android.util.Log;

import com.cnk.exceptions.DatabaseException;

import org.javatuples.Triplet;

import io.realm.Realm;
import io.realm.RealmResults;

public class DatabaseHelper {
    Context applicationContext;
    Realm realm;

    public DatabaseHelper(Context applicationContext) {
        this.applicationContext = applicationContext;
    }

    private void open() {
        realm = Realm.getInstance(applicationContext);
    }

    private void close() {
        realm.close();
    }

    public void beginTransaction() {
        realm.beginTransaction();
    }

    public void commitTransaction() {
        realm.commitTransaction();
    }

    public void cancelTransaction() {
        realm.cancelTransaction();
    }

    public Integer getVersion(Enum<Version.Item> item) throws DatabaseException {
        open();
        RealmResults<Version> results = realm.where(Version.class).equalTo("item", item.toString()).findAll();
        if (!results.isEmpty()) {
            Integer version = results.first().getCurrentVersion();
            close();
            return version;
        } else {
            close();
            throw new DatabaseException();
        }
    }

    public void setVersion(Enum<Version.Item> item, Integer currentVersion) throws DatabaseException {
        open();
        try {
            beginTransaction();
            Version version = new Version();
            version.setItem(item.toString());
            version.setCurrentVersion(currentVersion);
            realm.copyToRealmOrUpdate(version);
            commitTransaction();
            close();
        } catch (Exception e) {
            e.printStackTrace();
            cancelTransaction();
            close();
            throw new DatabaseException();
        }

    }

    public String getMapFile(Integer floor) throws DatabaseException {
        open();
        RealmResults<MapFile> results = realm.where(MapFile.class).equalTo("floor", floor).findAll();
        if (!results.isEmpty()) {
            String map = results.first().getMapFileLocation();
            close();
            return map;
        } else {
            close();
            throw new DatabaseException();
        }
    }

    public void setMapFile(Integer floor, String mapFileLocation) throws DatabaseException {
        try {
            open();
            beginTransaction();
            MapFile mapFile = new MapFile();
            mapFile.setFloor(floor);
            mapFile.setMapFileLocation(mapFileLocation);
            realm.copyToRealmOrUpdate(mapFile);
            commitTransaction();
            close();
        } catch (Exception e) {
            e.printStackTrace();
            close();
            throw new DatabaseException();
        }

    }

    public static final Integer floor1Code = 1;
    public static final Integer floor2Code = 2;

    public void setMap(Integer versionNum, String floor1MapLocation, String floor2MapLocation) throws DatabaseException {
        try {
            open();
            beginTransaction();
            setVersion(Version.Item.MAP, versionNum);
            setMapFile(floor1Code, floor1MapLocation);
            setMapFile(floor2Code, floor2MapLocation);
            commitTransaction();
            close();
        } catch (Exception e) {
            Log.e("Realm Error", "error msg: " + e.toString());
            cancelTransaction();
            close();
            throw new DatabaseException();
        }
    }

    public Triplet<Integer, String, String> getCurrentMapData() throws DatabaseException {
        Integer version;
        String floor1Map, floor2Map;
        open();
        try {
            beginTransaction();
            version = getVersion(Version.Item.MAP);
            floor1Map = getMapFile(floor1Code);
            floor2Map = getMapFile(floor2Code);
            commitTransaction();
            close();
            return Triplet.with(version, floor1Map, floor2Map);
        } catch (Exception e) {
            Log.e("Realm Error", "error msg: " + e.toString());
            cancelTransaction();
            close();
            throw new DatabaseException();
        }
    }
}
