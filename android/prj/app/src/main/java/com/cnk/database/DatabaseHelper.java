package com.cnk.database;

import android.content.Context;

import com.cnk.database.MapFileRealm;
import com.cnk.exceptions.DatabaseException;
import com.cnk.exceptions.InternalDatabaseError;


import io.realm.Realm;
import io.realm.RealmResults;

/*
 * impl functions could be used in both impl functions and public functions
 * public functions provide atomic transactions and should only be used from outside this class
 */
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

    private void beginTransaction() {
        realm.beginTransaction();
    }

    private void commitTransaction() {
        realm.commitTransaction();
    }

    private void cancelTransaction() {
        realm.cancelTransaction();
    }

    /*
     * returns current version of item if exists, otherwise null
     */
    private Integer getVersionImpl(Enum<Version.Item> item) {
        Integer versionNumber = null;

        RealmResults<VersionRealm> results = realm.where(VersionRealm.class).equalTo("item", item.toString()).findAll();
        if (!results.isEmpty()) {
            versionNumber = results.first().getCurrentVersion();
        }

        return versionNumber;
    }

    /*
     * returns current version of item if exists, otherwise null
     */
    public Integer getVersion(Enum<Version.Item> item) {
        Integer version = null;
        open();

        try {
            version = getVersionImpl(item);
        } catch (RuntimeException re) {
            re.printStackTrace();
            throw new InternalDatabaseError(re.toString());
        } finally {
            close();
        }

        return version;
    }

    private void setVersionImpl(Version.Item item, Integer versionNumber) {
        VersionRealm version = new VersionRealm();
        version.setItem(item.toString());
        version.setCurrentVersion(versionNumber);
        realm.copyToRealmOrUpdate(version);
    }

    public void setVersion(Version.Item item, Integer versionNumber) {
        open();

        try {
            beginTransaction();
            setVersionImpl(item, versionNumber);
            commitTransaction();
        } catch (RuntimeException e) {
            cancelTransaction();
            e.printStackTrace();
            throw new InternalDatabaseError("Exception setting version of " + item.toString()
                    + " to " + versionNumber.toString());
        } finally {
            close();
        }
    }

    /*
     * returns map file location for selected floor if exists, otherwise null
     */
    private String getMapFileImpl(Integer floor) {
        String mapFileAddr = null;

        RealmResults<MapFileRealm> results = realm.where(MapFileRealm.class).equalTo("floor", floor).findAll();
        if (!results.isEmpty()) {
            mapFileAddr = results.first().getMapFileLocation();
        }

        return mapFileAddr;
    }

    /*
     * returns map file location for selected floor if exists, otherwise null
     */
    public String getMapFile(Integer floor) throws DatabaseException {
        String mapFileAddr = null;
        open();

        try {
            mapFileAddr = getMapFileImpl(floor);
        } catch (RuntimeException re) {
            re.printStackTrace();
            throw new InternalDatabaseError(re.toString());
        } finally {
            close();
        }

        return mapFileAddr;
    }

    private void setMapFileImpl(Integer floor, String mapFileLocation) {
        MapFileRealm mapFileRealm = new MapFileRealm();
        mapFileRealm.setFloor(floor);
        mapFileRealm.setMapFileLocation(mapFileLocation);

        realm.copyToRealmOrUpdate(mapFileRealm);
    }

    public void setMapFile(Integer floor, String mapFileLocation) {
        open();

        try {
            beginTransaction();
            setMapFileImpl(floor, mapFileLocation);
            commitTransaction();
        } catch (Exception e) {
            cancelTransaction();
            e.printStackTrace();
            throw new InternalDatabaseError("Exception setting map of floor "
                    + floor.toString() + " to " + mapFileLocation);
        } finally {
            close();
        }
    }

    public static final Integer floor1Code = 1;
    public static final Integer floor2Code = 2;

    private void setMapsImpl(Integer versionNum, String floor1MapLocation, String floor2MapLocation) {
        setVersionImpl(Version.Item.MAP, versionNum);
        setMapFileImpl(floor1Code, floor1MapLocation);
        setMapFileImpl(floor2Code, floor2MapLocation);
    }

    public void setMaps(Integer versionNum, String floor1MapLocation, String floor2MapLocation) {
        open();

        try {
            beginTransaction();
            setMapsImpl(versionNum, floor1MapLocation, floor2MapLocation);
            commitTransaction();
        } catch (RuntimeException re) {
            cancelTransaction();
            re.printStackTrace();
            throw new InternalDatabaseError(re.toString());
        } finally {
            close();
        }
    }
}
