package com.cnk.database;

import android.content.Context;

import com.cnk.exceptions.InternalDatabaseError;

import java.util.ArrayList;
import java.util.List;

import io.realm.Realm;
import io.realm.RealmResults;

/*
 * impl functions could be used in both impl functions and public functions
 * public functions provide atomic transactions and should only be used from outside this class
 */
public class DatabaseHelper {
    Context applicationContext;

    public DatabaseHelper(Context applicationContext) {
        this.applicationContext = applicationContext;
    }

    private Realm open() {
        return Realm.getInstance(applicationContext);
    }

    private void close(Realm r) {
        r.close();
    }

    private void beginTransaction(Realm r) {
        r.beginTransaction();
    }

    private void commitTransaction(Realm r) {
        r.commitTransaction();
    }

    private void cancelTransaction(Realm r) {
        r.cancelTransaction();
    }

    /*
     * returns current version of item if exists, otherwise null
     */
    private Integer getVersionImpl(Realm r, Enum<Version.Item> item) {
        Integer versionNumber = null;

        VersionRealm result = r.where(VersionRealm.class).equalTo("item", item.toString()).findFirst();
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
        Realm r = open();

        try {
            beginTransaction(r);
            version = getVersionImpl(r, item);
            commitTransaction(r);
        } catch (RuntimeException re) {
            cancelTransaction(r);
            re.printStackTrace();
            throw new InternalDatabaseError(re.toString());
        } finally {
            close(r);
        }

        return version;
    }

    /*
     * sets version of certain item
     */
    private void setVersionImpl(Realm r, Version.Item item, Integer versionNumber) {
        VersionRealm version = new VersionRealm();
        version.setItem(item.toString());
        version.setCurrentVersion(versionNumber);
        r.copyToRealmOrUpdate(version);
    }

    /*
     * sets version of certain item
     */
    public void setVersion(Version.Item item, Integer versionNumber) {
        Realm r = open();

        try {
            beginTransaction(r);
            setVersionImpl(r, item, versionNumber);
            commitTransaction(r);
        } catch (RuntimeException re) {
            cancelTransaction(r);
            re.printStackTrace();
            throw new InternalDatabaseError("Exception setting version of " + item.toString()
                    + " to " + versionNumber.toString());
        } finally {
            close(r);
        }
    }

    /*
     * returns map file location for selected floor if exists, otherwise null
     */
    private String getMapFileImpl(Realm r, Integer floor) {
        String mapFileAddr = null;

        MapFileRealm result = r.where(MapFileRealm.class).equalTo("floor", floor).findFirst();
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
        Realm r = open();

        try {
            beginTransaction(r);
            mapFileAddr = getMapFileImpl(r, floor);
            commitTransaction(r);
        } catch (RuntimeException re) {
            cancelTransaction(r);
            re.printStackTrace();
            throw new InternalDatabaseError(re.toString());
        } finally {
            close(r);
        }

        return mapFileAddr;
    }

    /*
     * sets map file location for certain floor
     */
    private void setMapFileImpl(Realm r, Integer floor, String mapFileLocation) {
        MapFileRealm mapFileRealm = new MapFileRealm();
        mapFileRealm.setFloor(floor);
        mapFileRealm.setMapFileLocation(mapFileLocation);

        r.copyToRealmOrUpdate(mapFileRealm);
    }

    /*
     * sets map file location for certain floor
     */
    public void setMapFile(Integer floor, String mapFileLocation) {
        Realm r = open();

        try {
            beginTransaction(r);
            setMapFileImpl(r, floor, mapFileLocation);
            commitTransaction(r);
        } catch (Exception e) {
            cancelTransaction(r);
            e.printStackTrace();
            throw new InternalDatabaseError("Exception setting map of floor "
                    + floor.toString() + " to " + mapFileLocation);
        } finally {
            close(r);
        }
    }

    public static final Integer floor0Code = 0;
    public static final Integer floor1Code = 1;

    /*
     * sets maps for both floors, if passed map is null, then it's ignored
     */
    private void setMapsImpl(Realm r, Integer versionNum, String floor1MapLocation, String floor2MapLocation) {
        assert(versionNum != null);

        setVersionImpl(r, Version.Item.MAP, versionNum);
        if (floor1MapLocation != null) {
            setMapFileImpl(r, floor0Code, floor1MapLocation);
        }
        if (floor2MapLocation != null) {
            setMapFileImpl(r, floor1Code, floor2MapLocation);
        }
    }

    /*
     * sets maps for both floors, if one of maps is null, then it's ignored
     */
    public void setMaps(Integer versionNum, String floor0MapLocation, String floor1MapLocation) {
        Realm r = open();

        try {
            beginTransaction(r);
            setMapsImpl(r, versionNum, floor0MapLocation, floor1MapLocation);
            commitTransaction(r);
        } catch (RuntimeException re) {
            cancelTransaction(r);
            re.printStackTrace();
            throw new InternalDatabaseError(re.toString());
        } finally {
            close(r);
        }
    }

    /*
     * returns exhibit with specific id if exists, otherwise null
     */
    private Exhibit getExhibitImpl(Realm r, Integer id) {
        ExhibitRealm result = r.where(ExhibitRealm.class).equalTo("id", id).findFirst();

        if (result != null) {
            return ModelTranslation.exhibitFromRealm(result);
        } else {
            return null;
        }
    }

    /*
     * returns exhibit with specific id if exists, otherwise null
     */
    public Exhibit getExhibit(Integer id) {
        Exhibit exhibit = null;
        Realm r = open();

        try {
            beginTransaction(r);
            exhibit = getExhibitImpl(r, id);
            commitTransaction(r);
        } catch (RuntimeException re) {
            cancelTransaction(r);
            re.printStackTrace();
            throw new InternalDatabaseError(re.toString());
        } finally {
            close(r);
        }

        return exhibit;
    }

    /*
     * if exhibit with such id exists, updates it, otherwise adds new exhibit
     */
    private void setExhibitImpl(Realm r, Exhibit e) {
        ExhibitRealm er = ModelTranslation.realmFromExhibit(e);
        r.copyToRealmOrUpdate(er);
    }

    /*
     * if exhibit with such id exists, updates it, otherwise adds new exhibit
     */
    public void setExhibit(Integer versionNum, Exhibit e) {
        Realm r = open();

        try {
            beginTransaction(r);
            setVersionImpl(r, Version.Item.EXHIBITS, versionNum);
            setExhibitImpl(r, e);
            commitTransaction(r);
        } catch (RuntimeException re) {
            cancelTransaction(r);
            re.printStackTrace();
            throw new InternalDatabaseError("Exception saving exhibit with id " + e.getId());
        } finally {
            close(r);
        }
    }

    /*
     * return list of all exhibits in database, if there are no exhibits, list is empty
     */
    private List<Exhibit> getAllExhibitsImpl(Realm r) {
        List<Exhibit> exhibits = new ArrayList<>();
        RealmResults<ExhibitRealm> results = r.allObjects(ExhibitRealm.class);

        for (ExhibitRealm er : results) {
            exhibits.add(ModelTranslation.exhibitFromRealm(er));
        }

        return exhibits;
    }

    /*
     * return list of all exhibits in database, if there are no exhibits, list is empty
     */
    public List<Exhibit> getAllExhibits() {
        List<Exhibit> exhibits = null;
        Realm r = open();

        try {
            beginTransaction(r);
            exhibits = getAllExhibitsImpl(r);
            commitTransaction(r);
        } catch (RuntimeException re) {
            cancelTransaction(r);
            re.printStackTrace();
            throw new InternalDatabaseError(re.toString());
        } finally {
            close(r);
        }

        return exhibits;
    }

    /*
     * removes all exhbits on certain floor and sets passed exhbits as exhibits for that floor
     */
    private void setAllExhibitsImpl(Realm r, Iterable<Exhibit> allExhibits) {
        r.clear(ExhibitRealm.class);

        for (Exhibit e : allExhibits) {
            r.copyToRealmOrUpdate(ModelTranslation.realmFromExhibit(e));
        }
    }

    /*
     * removes all exhbits on certain floor and sets passed exhbits as exhibits for that floor
     */
    public void setAllExhibits(Integer versionNum, Iterable<Exhibit> allExhibits) {
        Realm r = open();

        try {
            beginTransaction(r);
            setVersionImpl(r, Version.Item.EXHIBITS, versionNum);
            setAllExhibitsImpl(r, allExhibits);
            commitTransaction(r);
        } catch (RuntimeException re) {
            cancelTransaction(r);
            re.printStackTrace();
            throw new InternalDatabaseError("Exception setting all exhibits.");
        } finally {
            close(r);
        }
    }

    /*
     * return list of all exhibits for floor, if there are no exhibits, list is empty
     */
    private List<Exhibit> getAllExhibitsForFloorImpl(Realm r, Integer floor) {
        List<Exhibit> exhibits = new ArrayList<>();
        RealmResults<ExhibitRealm> results =
                r.where(ExhibitRealm.class).equalTo("floor", floor).findAll();

        for (ExhibitRealm er : results) {
            exhibits.add(ModelTranslation.exhibitFromRealm(er));
        }

        return exhibits;
    }

    /*
     * return list of all exhibits for floor, if there are no exhibits, list is empty
     */
    public List<Exhibit> getAllExhibitsForFloor(Integer floor) {
        List<Exhibit> exhibits = null;
        Realm r = open();

        try {
            beginTransaction(r);
            exhibits = getAllExhibitsForFloorImpl(r, floor);
            commitTransaction(r);
        } catch (RuntimeException re) {
            cancelTransaction(r);
            re.printStackTrace();
            throw new InternalDatabaseError("Exception getting all exhibits for floor. " + re.toString());
        } finally {
            close(r);
        }

        return exhibits;
    }

    /*
     * clears exhbits on certain floor and sets passed exhbits as exhibits for that floor
     */
    private void setExhibitsForFloorImpl(Realm r, Iterable<Exhibit> exhibitsForFloor, Integer floor) {
        RealmResults<ExhibitRealm> exhibitsToDelete =
                r.where(ExhibitRealm.class).equalTo("floor", floor).findAll();
        exhibitsToDelete.clear();

        for (Exhibit e : exhibitsForFloor) {
            r.copyToRealmOrUpdate(ModelTranslation.realmFromExhibit(e));
        }
    }

    /*
     * clears exhbits on certain floor and sets passed exhbits as exhibits for that floor
     */
    public void setExhibitsForFloor(Integer versionNum, Iterable<Exhibit> exhibitsForFloor, Integer floor) {
        Realm r = open();

        try {
            beginTransaction(r);
            setVersionImpl(r, Version.Item.EXHIBITS, versionNum);
            setExhibitsForFloorImpl(r, exhibitsForFloor, floor);
            commitTransaction(r);
        } catch (RuntimeException re) {
            cancelTransaction(r);
            re.printStackTrace();
            throw new InternalDatabaseError("Exception saving exhibits for floor: " + floor.toString());
        } finally {
            close(r);
        }
    }

    /*
     * for each exhibit from list adds it if its id doesn't exhist in database,
     * otherwise updates exhibit with such id
     */
    private void addOrUpdateExhibitsImpl(Realm r, Iterable<Exhibit> exhibits) {
        r.copyToRealmOrUpdate(ModelTranslation.realmFromExhibits(exhibits));
    }

    /*
     * for each exhibit from argument adds it if its id doesn't exhist in database,
     * otherwise updates exhibit with such id
     */
    public void addOrUpdateExhibits(Integer versionNum, Iterable<Exhibit> exhibits) {
        Realm r = open();

        try {
            beginTransaction(r);
            setVersionImpl(r, Version.Item.EXHIBITS, versionNum);
            addOrUpdateExhibitsImpl(r, exhibits);
            commitTransaction(r);
        } catch (RuntimeException re) {
            cancelTransaction(r);
            re.printStackTrace();
            throw new InternalDatabaseError("Exception adding or updating multiple exhibits.");
        } finally {
            close(r);
        }
    }

}
