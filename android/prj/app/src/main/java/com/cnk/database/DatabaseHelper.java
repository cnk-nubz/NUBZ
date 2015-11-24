package com.cnk.database;

import android.content.Context;

import com.cnk.exceptions.InternalDatabaseError;

import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;

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

        VersionRealm result = realm.where(VersionRealm.class).equalTo("item", item.toString()).findFirst();
        if (result != null) {
            versionNumber = result.getCurrentVersion();
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
        } catch (RuntimeException re) {
            cancelTransaction();
            re.printStackTrace();
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

        MapFileRealm result = realm.where(MapFileRealm.class).equalTo("floor", floor).findFirst();
        if (result != null) {
            mapFileAddr = result.getMapFileLocation();
        }

        return mapFileAddr;
    }

    /*
     * returns map file location for selected floor if exists, otherwise null
     */
    public String getMapFile(Integer floor) {
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

    private Exhibit getExhibitImpl(Integer id) {
        ExhibitRealm result = realm.where(ExhibitRealm.class).equalTo("id", id).findFirst();

        return ModelTranslation.exhibitFromRealm(result);
    }

    public Exhibit getExhibit(Integer id) {
        Exhibit exhibit = null;
        open();

        try {
            exhibit = getExhibitImpl(id);
        } catch (RuntimeException re) {
            re.printStackTrace();
            throw new InternalDatabaseError(re.toString());
        } finally {
            close();
        }

        return exhibit;
    }

    private void setExhibitImpl(Integer id, com.cnk.communication.Exhibit e) {
        ExhibitRealm er = ModelTranslation.realmFromExhibit(new Exhibit(id, e));
        realm.copyToRealmOrUpdate(er);
    }

    public void setExhibit(Integer id, com.cnk.communication.Exhibit e) {
        open();

        try {
            beginTransaction();
            setExhibitImpl(id, e);
            commitTransaction();
        } catch (RuntimeException re) {
            cancelTransaction();
            re.printStackTrace();
            throw new InternalDatabaseError("Exception saving exhibit with id " + id.toString());
        } finally {
            close();
        }
    }

    private List<Exhibit> getAllExhibitsImpl() {
        List<Exhibit> exhibits = new ArrayList<>();
        RealmResults<ExhibitRealm> results = realm.allObjects(ExhibitRealm.class);

        for (ExhibitRealm er : results) {
            exhibits.add(ModelTranslation.exhibitFromRealm(er));
        }

        return exhibits;
    }

    public List<Exhibit> getAllExhibits() {
        List<Exhibit> exhibits = null;
        open();

        try {
            beginTransaction();
            exhibits = getAllExhibitsImpl();
            commitTransaction();
        } catch (RuntimeException re) {
            cancelTransaction();
            re.printStackTrace();
            throw new InternalDatabaseError(re.toString());
        } finally {
            close();
        }

        return exhibits;
    }

    private void setAllExhibitsImpl(Iterable<Exhibit> allExhibits) {
        realm.clear(ExhibitRealm.class);

        for (Exhibit e : allExhibits) {
            realm.copyToRealmOrUpdate(ModelTranslation.realmFromExhibit(e));
        }
    }

    public void setAllExhibits(Iterable<Exhibit> allExhibits) {
        open();

        try {
            beginTransaction();
            setAllExhibitsImpl(allExhibits);
            commitTransaction();
        } catch (RuntimeException re) {
            cancelTransaction();
            re.printStackTrace();
            throw new InternalDatabaseError("Exception saving all exhibits.");
        } finally {
            close();
        }
    }

    private List<Exhibit> getAllExhibitsForFloorImpl(Integer floor) {
        List<Exhibit> exhibits = new ArrayList<>();
        RealmResults<ExhibitRealm> results =
                realm.where(ExhibitRealm.class).equalTo("floor", floor).findAll();

        for (ExhibitRealm er : results) {
            exhibits.add(ModelTranslation.exhibitFromRealm(er));
        }

        return exhibits;
    }

    public List<Exhibit> getAllExhibitsForFloor(Integer floor) {
        List<Exhibit> exhibits = null;
        open();

        try {
            beginTransaction();
            exhibits = getAllExhibitsForFloorImpl(floor);
            commitTransaction();
        } catch (RuntimeException re) {
            cancelTransaction();
            re.printStackTrace();
            throw new InternalDatabaseError(re.toString());
        } finally {
            close();
        }

        return exhibits;
    }

    private void setExhibitsForFloorImpl(Iterable<Exhibit> exhibitsForFloor, Integer floor) {
        RealmResults<ExhibitRealm> exhibitsToDelete =
                realm.where(ExhibitRealm.class).equalTo("floor", floor).findAll();
        exhibitsToDelete.clear();

        for (Exhibit e : exhibitsForFloor) {
            realm.copyToRealmOrUpdate(ModelTranslation.realmFromExhibit(e));
        }
    }

    public void setExhibitsForFloor(Iterable<Exhibit> exhibitsForFloor, Integer floor) {
        open();

        try {
            beginTransaction();
            setExhibitsForFloorImpl(exhibitsForFloor, floor);
            commitTransaction();
        } catch (RuntimeException re) {
            cancelTransaction();
            re.printStackTrace();
            throw new InternalDatabaseError("Exception saving exhibits for floor: " + floor.toString());
        } finally {
            close();
        }
    }

}
