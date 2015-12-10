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
            beginTransaction();
            version = getVersionImpl(item);
            commitTransaction();
        } catch (RuntimeException re) {
            cancelTransaction();
            re.printStackTrace();
            throw new InternalDatabaseError(re.toString());
        } finally {
            close();
        }

        return version;
    }

    /*
     * sets version of certain item
     */
    private void setVersionImpl(Version.Item item, Integer versionNumber) {
        VersionRealm version = new VersionRealm();
        version.setItem(item.toString());
        version.setCurrentVersion(versionNumber);
        realm.copyToRealmOrUpdate(version);
    }

    /*
     * sets version of certain item
     */
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
            beginTransaction();
            mapFileAddr = getMapFileImpl(floor);
            commitTransaction();
        } catch (RuntimeException re) {
            cancelTransaction();
            re.printStackTrace();
            throw new InternalDatabaseError(re.toString());
        } finally {
            close();
        }

        return mapFileAddr;
    }

    /*
     * sets map file location for certain floor
     */
    private void setMapFileImpl(Integer floor, String mapFileLocation) {
        MapFileRealm mapFileRealm = new MapFileRealm();
        mapFileRealm.setFloor(floor);
        mapFileRealm.setMapFileLocation(mapFileLocation);

        realm.copyToRealmOrUpdate(mapFileRealm);
    }

    /*
     * sets map file location for certain floor
     */
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

    public static final Integer floor0Code = 0;
    public static final Integer floor1Code = 1;

    /*
     * sets maps for both floors, if passed map is null, then it's ignored
     */
    private void setMapsImpl(Integer versionNum, String floor1MapLocation, String floor2MapLocation) {
        assert(versionNum != null);

        setVersionImpl(Version.Item.MAP, versionNum);
        if (floor1MapLocation != null) {
            setMapFileImpl(floor0Code, floor1MapLocation);
        }
        if (floor2MapLocation != null) {
            setMapFileImpl(floor1Code, floor2MapLocation);
        }
    }

    /*
     * sets maps for both floors, if one of maps is null, then it's ignored
     */
    public void setMaps(Integer versionNum, String floor0MapLocation, String floor1MapLocation) {
        open();

        try {
            beginTransaction();
            setMapsImpl(versionNum, floor0MapLocation, floor1MapLocation);
            commitTransaction();
        } catch (RuntimeException re) {
            cancelTransaction();
            re.printStackTrace();
            throw new InternalDatabaseError(re.toString());
        } finally {
            close();
        }
    }

    /*
     * returns exhibit with specific id if exists, otherwise null
     */
    private Exhibit getExhibitImpl(Integer id) {
        ExhibitRealm result = realm.where(ExhibitRealm.class).equalTo("id", id).findFirst();

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
        open();

        try {
            beginTransaction();
            exhibit = getExhibitImpl(id);
            commitTransaction();
        } catch (RuntimeException re) {
            cancelTransaction();
            re.printStackTrace();
            throw new InternalDatabaseError(re.toString());
        } finally {
            close();
        }

        return exhibit;
    }

    /*
     * if exhibit with such id exists, updates it, otherwise adds new exhibit
     */
    private void setExhibitImpl(Exhibit e) {
        ExhibitRealm er = ModelTranslation.realmFromExhibit(e);
        realm.copyToRealmOrUpdate(er);
    }

    /*
     * if exhibit with such id exists, updates it, otherwise adds new exhibit
     */
    public void setExhibit(Integer versionNum, Exhibit e) {
        open();

        try {
            beginTransaction();
            setVersionImpl(Version.Item.EXHIBITS, versionNum);
            setExhibitImpl(e);
            commitTransaction();
        } catch (RuntimeException re) {
            cancelTransaction();
            re.printStackTrace();
            throw new InternalDatabaseError("Exception saving exhibit with id " + e.getId());
        } finally {
            close();
        }
    }

    /*
     * return list of all exhibits in database, if there are no exhibits, list is empty
     */
    private List<Exhibit> getAllExhibitsImpl() {
        List<Exhibit> exhibits = new ArrayList<>();
        RealmResults<ExhibitRealm> results = realm.allObjects(ExhibitRealm.class);

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

    /*
     * removes all exhbits on certain floor and sets passed exhbits as exhibits for that floor
     */
    private void setAllExhibitsImpl(Iterable<Exhibit> allExhibits) {
        realm.clear(ExhibitRealm.class);

        for (Exhibit e : allExhibits) {
            realm.copyToRealmOrUpdate(ModelTranslation.realmFromExhibit(e));
        }
    }

    /*
     * removes all exhbits on certain floor and sets passed exhbits as exhibits for that floor
     */
    public void setAllExhibits(Integer versionNum, Iterable<Exhibit> allExhibits) {
        open();

        try {
            beginTransaction();
            setVersionImpl(Version.Item.EXHIBITS, versionNum);
            setAllExhibitsImpl(allExhibits);
            commitTransaction();
        } catch (RuntimeException re) {
            cancelTransaction();
            re.printStackTrace();
            throw new InternalDatabaseError("Exception setting all exhibits.");
        } finally {
            close();
        }
    }

    /*
     * return list of all exhibits for floor, if there are no exhibits, list is empty
     */
    private List<Exhibit> getAllExhibitsForFloorImpl(Integer floor) {
        List<Exhibit> exhibits = new ArrayList<>();
        RealmResults<ExhibitRealm> results =
                realm.where(ExhibitRealm.class).equalTo("floor", floor).findAll();

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
        open();

        try {
            beginTransaction();
            exhibits = getAllExhibitsForFloorImpl(floor);
            commitTransaction();
        } catch (RuntimeException re) {
            cancelTransaction();
            re.printStackTrace();
            throw new InternalDatabaseError("Exception getting all exhibits for floor. " + re.toString());
        } finally {
            close();
        }

        return exhibits;
    }

    /*
     * clears exhbits on certain floor and sets passed exhbits as exhibits for that floor
     */
    private void setExhibitsForFloorImpl(Iterable<Exhibit> exhibitsForFloor, Integer floor) {
        RealmResults<ExhibitRealm> exhibitsToDelete =
                realm.where(ExhibitRealm.class).equalTo("floor", floor).findAll();
        exhibitsToDelete.clear();

        for (Exhibit e : exhibitsForFloor) {
            realm.copyToRealmOrUpdate(ModelTranslation.realmFromExhibit(e));
        }
    }

    /*
     * clears exhbits on certain floor and sets passed exhbits as exhibits for that floor
     */
    public void setExhibitsForFloor(Integer versionNum, Iterable<Exhibit> exhibitsForFloor, Integer floor) {
        open();

        try {
            beginTransaction();
            setVersionImpl(Version.Item.EXHIBITS, versionNum);
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

    /*
     * for each exhibit from list adds it if its id doesn't exhist in database,
     * otherwise updates exhibit with such id
     */
    private void addOrUpdateExhibitsRealm(Iterable<Exhibit> exhibits) {
        realm.copyToRealmOrUpdate(ModelTranslation.realmFromExhibits(exhibits));
    }

    /*
     * for each exhibit from argument adds it if its id doesn't exhist in database,
     * otherwise updates exhibit with such id
     */
    public void addOrUpdateExhibits(Integer versionNum, Iterable<Exhibit> exhibits) {
        open();

        try {
            beginTransaction();
            setVersionImpl(Version.Item.EXHIBITS, versionNum);
            addOrUpdateExhibitsRealm(exhibits);
            commitTransaction();
        } catch (RuntimeException re) {
            cancelTransaction();
            re.printStackTrace();
            throw new InternalDatabaseError("Exception adding or updating multiple exhibits.");
        } finally {
            close();
        }
    }

    private Integer getNextRaportIdImpl() {
        Number maxId = realm.where(RaportFileRealm.class).max("id");
        return maxId == null ? 0 : maxId.intValue() + 1;
    }

    public Integer getNextRaportId() {
        open();
        Integer result;
        try {
            beginTransaction();
            result = getNextRaportIdImpl();
            commitTransaction();
        } catch (RuntimeException re) {
            cancelTransaction();
            re.printStackTrace();
            throw new InternalDatabaseError("Exception getting next raport id.");
        } finally {
            close();
        }
        return result;
    }

    private String getRaportFilenameImpl(Integer id) {
        RaportFileRealm result = realm.where(RaportFileRealm.class).equalTo("id", id).findFirst();
        return result == null ? null : result.getFileName();
    }

    public String getRaportFilename(Integer id) {
        String raportFile = null;
        open();

        try {
            beginTransaction();
            raportFile = getRaportFilenameImpl(id);
            commitTransaction();
        } catch (RuntimeException re) {
            cancelTransaction();
            re.printStackTrace();
            throw new InternalDatabaseError(re.toString());
        } finally {
            close();
        }

        return raportFile;
    }

    private void setRaportFileImpl(Integer id, String raportFilename) {
        RaportFileRealm raportFileRealm = new RaportFileRealm();
        raportFileRealm.setId(id);
        raportFileRealm.setFileName(raportFilename);
        raportFileRealm.setState(RaportFileRealm.IN_PROGRESS);
        raportFileRealm.setServerId(null);
        realm.copyToRealmOrUpdate(raportFileRealm);
    }

    public void setRaportFile(Integer id, String raportFilename) {
        open();

        try {
            beginTransaction();
            setRaportFileImpl(id, raportFilename);
            commitTransaction();
        } catch (Exception e) {
            cancelTransaction();
            e.printStackTrace();
            throw new InternalDatabaseError("Exception setting map of floor "
                    + id.toString() + " to " + raportFilename);
        } finally {
            close();
        }
    }

    private List<RaportFile> getAllReadyRaportsImpl() {
        List<RaportFile> raports = new ArrayList<>();
        RealmResults<RaportFileRealm> results = realm.where(RaportFileRealm.class).equalTo("state", RaportFileRealm.READY_TO_SEND).findAll();
        for (RaportFileRealm r : results) {
            raports.add(ModelTranslation.raportFileFromRealm(r));
        }
        return raports;
    }

    public List<RaportFile> getAllReadyRaports() {
        List<RaportFile> raports = new ArrayList<>();
        open();

        try {
            beginTransaction();
            raports = getAllReadyRaportsImpl();
            commitTransaction();
        } catch (RuntimeException re) {
            cancelTransaction();
            re.printStackTrace();
            throw new InternalDatabaseError(re.toString());
        } finally {
            close();
        }

        return raports;
    }

    private void changeRaportStateImpl(Integer id, String newState) {
        RaportFileRealm entry = realm.where(RaportFileRealm.class).equalTo("id", id).findFirst();
        entry.setState(newState);
        realm.copyToRealmOrUpdate(entry);
    }

    public void changeRaportState(Integer id, String newState) {
        open();

        try {
            beginTransaction();
            changeRaportStateImpl(id, newState);
            commitTransaction();
        } catch (RuntimeException re) {
            cancelTransaction();
            re.printStackTrace();
            throw new InternalDatabaseError(re.toString());
        } finally {
            close();
        }
    }

    private void changeRaportServerIdImpl(Integer id, Integer serverId) {
        RaportFileRealm entry = realm.where(RaportFileRealm.class).equalTo("id", id).findFirst();
        entry.setServerId(serverId);
        realm.copyToRealmOrUpdate(entry);
    }

    public void changeRaportServerId(Integer id, Integer serverId) {
        open();

        try {
            beginTransaction();
            changeRaportServerIdImpl(id, serverId);
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
