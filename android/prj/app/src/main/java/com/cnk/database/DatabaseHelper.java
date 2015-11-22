package com.cnk.database;

import android.content.Context;
import android.util.Log;

import com.cnk.database.MapFileRealm;
import com.cnk.exceptions.DatabaseException;
import com.cnk.exceptions.InternalDatabaseError;

import org.javatuples.Triplet;

import io.realm.Realm;
import io.realm.RealmResults;
import io.realm.internal.IOException;

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

    private Integer getVersionImpl(Enum<Version.Item> item) {
        Integer versionNumber = null;

        RealmResults<VersionRealm> results = realm.where(VersionRealm.class).equalTo("item", item.toString()).findAll();
        if (!results.isEmpty()) {
            versionNumber = results.first().getCurrentVersion();
        }

        return versionNumber;
    }

    public Integer getVersion(Enum<Version.Item> item) {
        Integer version;
        open();

        try {
            version = getVersionImpl(item);
        } catch (RuntimeException re) {
            re.printStackTrace();
            throw new InternalDatabaseError(re.toString());
        } finally {
            close();
        }

        if (version == null) {
            throw new InternalDatabaseError("No such element: " + item.toString());
        } else {
            return version;
        }
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

    private String getMapFileImpl(Integer floor) {
        String mapFileAddr = null;

        RealmResults<MapFileRealm> results = realm.where(MapFileRealm.class).equalTo("floor", floor).findAll();
        if (!results.isEmpty()) {
            mapFileAddr = results.first().getMapFileLocation();
        }

        return mapFileAddr;
    }

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

        if (mapFileAddr == null) {
            throw new InternalDatabaseError("No map for such floor: " + floor.toString());
        } else {
            return mapFileAddr;
        }
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

    private Triplet<Integer, String, String> getCurrentMapDataImpl() {
        Integer version;
        String floor1Map, floor2Map;

        version = getVersionImpl(Version.Item.MAP);
        floor1Map = getMapFileImpl(floor1Code);
        floor2Map = getMapFileImpl(floor2Code);

        return Triplet.with(version, floor1Map, floor2Map);
    }

    public Triplet<Integer, String, String> getCurrentMapData() {
        Triplet<Integer, String, String> mapDataTriplet;
        open();

        try {
            beginTransaction();
            mapDataTriplet = getCurrentMapDataImpl();
            commitTransaction();
        } catch (RuntimeException re) {
            cancelTransaction();
            re.printStackTrace();
            throw new InternalDatabaseError(re.toString());
        } finally {
            close();
        }

        if (mapDataTriplet.getValue0() == null) {
            throw new InternalDatabaseError("No such element: " + Version.Item.MAP.toString());
        } else if (mapDataTriplet.getValue1() == null) {
            throw new InternalDatabaseError("No map for such floor: " + floor1Code.toString());
        } else if (mapDataTriplet.getValue2() == null) {
            throw new InternalDatabaseError("No map for such floor: " + floor2Code.toString());
        } else {
            return mapDataTriplet;
        }
    }
}
