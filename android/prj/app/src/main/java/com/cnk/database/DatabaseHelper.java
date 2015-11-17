package com.cnk.database;

import android.content.Context;
import android.util.Log;

import com.cnk.exceptions.NoSuchRecordException;

import org.javatuples.Triplet;

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

    public void beginTransaction() {
        realm.beginTransaction();
    }

    public void commitTransaction() {
        realm.commitTransaction();
    }

    public void cancelTransaction() {
        realm.cancelTransaction();
    }

    public Version getVersion(Enum<Version.Item> item) throws NoSuchRecordException {
        RealmResults<Version> results = realm.where(Version.class)
                .equalTo("item", item.toString()).findAll();

        if (!results.isEmpty()) {
            return results.first();
        } else {
            throw new NoSuchRecordException();
        }
    }

    public void setVersion(Enum<Version.Item> item, Integer currentVersion) throws RuntimeException {
        Version version = new Version();
        version.setItem(item.toString());
        version.setCurrentVersion(currentVersion);

        realm.copyToRealmOrUpdate(version);
    }

    public MapFile getMapFile(Integer floor) throws NoSuchRecordException {
        RealmResults<MapFile> results = realm.where(MapFile.class)
                .equalTo("floor", floor).findAll();

        if (!results.isEmpty()) {
            return results.first();
        } else {
            throw new NoSuchRecordException();
        }
    }

    public void setMapFile(Integer floor, String mapFileLocation) throws RuntimeException {
        MapFile mapFile = new MapFile();
        mapFile.setFloor(floor);
        mapFile.setMapFileLocation(mapFileLocation);

        realm.copyToRealmOrUpdate(mapFile);
    }

    public static final Integer floor1Code = 1;
    public static final Integer floor2Code = 2;

    public void setMap(Integer versionNum, String floor1MapLocation, String floor2MapLocation) throws RuntimeException {
        try {
            beginTransaction();
            setVersion(Version.Item.MAP, versionNum);
            setMapFile(floor1Code, floor1MapLocation);
            setMapFile(floor2Code, floor2MapLocation);
            commitTransaction();
        } catch (Exception e) {
            Log.e("Realm Error", "error msg: " + e.toString());
            cancelTransaction();
            throw e;
        }
    }

    public Triplet<Integer, String, String> getCurrentMapData() throws RuntimeException {
        Integer version;
        String floor1Map, floor2Map;
        try {
            beginTransaction();
            version = getVersion(Version.Item.MAP).getCurrentVersion();
            floor1Map = getMapFile(floor1Code).getMapFileLocation();
            floor2Map = getMapFile(floor2Code).getMapFileLocation();
            cancelTransaction();
            return Triplet.with(version, floor1Map, floor2Map);
        } catch (Exception e) {
            Log.e("Realm Error", "error msg: " + e.toString());
            cancelTransaction();
            throw e;
        }

    }
}
