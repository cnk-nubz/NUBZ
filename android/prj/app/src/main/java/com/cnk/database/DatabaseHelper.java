package com.cnk.database;

import android.content.Context;

import com.cnk.data.FloorMap;
import com.cnk.database.models.DetailLevelRes;
import com.cnk.database.models.Exhibit;
import com.cnk.database.models.FloorDetailLevels;
import com.cnk.database.models.RaportFile;
import com.cnk.database.models.Version;
import com.cnk.database.realm.DetailLevelResRealm;
import com.cnk.database.realm.ExhibitRealm;
import com.cnk.database.realm.FloorDetailLevelsRealm;
import com.cnk.database.realm.MapTileRealm;
import com.cnk.database.realm.RaportFileRealm;
import com.cnk.database.realm.VersionRealm;
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

    private void close(Realm realm) {
        realm.close();
    }

    private void beginTransaction(Realm realm) {
        realm.beginTransaction();
    }

    private void commitTransaction(Realm realm) {
        realm.commitTransaction();
    }

    private void cancelTransaction(Realm realm) {
        realm.cancelTransaction();
    }

    /*
     * returns current version of item if exists, otherwise null
     */
    private Integer getVersionImpl(Realm realm, Enum<Version.Item> item) {
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
    private void setVersionImpl(Realm realm, Version.Item item, Integer versionNumber) {
        VersionRealm version = new VersionRealm();
        version.setItem(item.toString());
        version.setCurrentVersion(versionNumber);
        realm.copyToRealmOrUpdate(version);
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

    public static final Integer floor0Code = 0;
    public static final Integer floor1Code = 1;

    public DetailLevelResRealm getDetailLevelResRealmImpl(Realm realm, Integer floor, Integer detailLevel) {
        List<DetailLevelResRealm> list = realm.where(DetailLevelResRealm.class).findAll();

        DetailLevelResRealm result = realm.where(DetailLevelResRealm.class).equalTo("floor", floor)
                .equalTo("detailLevel", detailLevel).findFirst();

        return result;
    }

    public DetailLevelRes getDetailLevelRes(Integer floor, Integer detailLevel) {
        DetailLevelRes result = null;

        Realm r = open();
        try {
            beginTransaction(r);
            result = ModelTranslation.detailLevelResFromRealm(getDetailLevelResRealmImpl(r, floor, detailLevel));
            commitTransaction(r);
        } catch (RuntimeException re) {
            cancelTransaction(r);
            re.printStackTrace();
            throw new InternalDatabaseError(re.toString());
        } finally {
            close(r);
        }

        return result;
    }

    private void setMapResolutionsImpl(Realm realm,
                                       List<DetailLevelResRealm> floor0Resolutions,
                                       List<DetailLevelResRealm> floor1Resolutions) {
        if (floor0Resolutions != null) {
            realm.where(DetailLevelResRealm.class).equalTo("floor", floor0Code).findAll().clear();
            realm.copyToRealm(floor0Resolutions);
        }
        if (floor1Resolutions != null) {
            realm.where(DetailLevelResRealm.class).equalTo("floor", floor1Code).findAll().clear();
            realm.copyToRealm(floor1Resolutions);
        }
    }

    private String getMapTileFileLocationImpl(Realm realm, Integer floor, Integer detailLevel,
                                              Integer rowNumber, Integer colunNumber) {
        String result = null;

        MapTileRealm mtr = realm.where(MapTileRealm.class).equalTo("floor", floor)
                .equalTo("detailLevel", detailLevel).equalTo("rowNumber", rowNumber)
                .equalTo("columnNumber", colunNumber).findFirst();

        if (mtr != null) {
            result = mtr.getMapTileLocation();
        }

        return result;
    }

    public String getMapTileFileLocation(Integer floor, Integer detailLevel,
                                         Integer rowNumber, Integer columnNumber) {
        String result = null;

        Realm r = open();
        try {
            beginTransaction(r);
            result = getMapTileFileLocationImpl(r, floor, detailLevel, rowNumber, columnNumber);
            commitTransaction(r);
        } catch (RuntimeException re) {
            cancelTransaction(r);
            re.printStackTrace();
            throw new InternalDatabaseError(re.toString());
        } finally {
            close(r);
        }

        return result;
    }

    private void setDetailLevelsImpl(Realm realm, FloorDetailLevelsRealm floor0, FloorDetailLevelsRealm floor1) {
        if (floor0 != null) {
            realm.where(FloorDetailLevelsRealm.class).equalTo("floorNo", floor0.getFloorNo()).findAll().clear();
            realm.copyToRealm(floor0);
        }
        if (floor1 != null) {
            realm.where(FloorDetailLevelsRealm.class).equalTo("floorNo", floor1.getFloorNo()).findAll().clear();
            realm.copyToRealm(floor1);
        }

    }

    private void setMapsImpl(Realm realm, Integer versionNum, List<MapTileRealm> tilesForFloor0,
                            List<MapTileRealm> tilesForFloor1) {
        assert(versionNum != null);

        setVersionImpl(realm, Version.Item.MAP, versionNum);
        if (tilesForFloor0 != null) {
            realm.where(MapTileRealm.class).equalTo("floor", floor0Code).findAll().clear();
            realm.copyToRealm(tilesForFloor0);
        }
        if (tilesForFloor1 != null) {
            realm.where(MapTileRealm.class).equalTo("floor", floor1Code).findAll().clear();
            realm.copyToRealm(tilesForFloor1);
        }
    }

    public void setMaps(Integer versionNum, FloorMap floor0Map, FloorMap floor1Map) {
        List<MapTileRealm> floor0Tiles = null;
        List<MapTileRealm> floor1Tiles = null;
        List<DetailLevelResRealm> floor0Resolutions = null;
        List<DetailLevelResRealm> floor1Resolutions = null;
        FloorDetailLevelsRealm floor0DetailLevels = null;
        FloorDetailLevelsRealm floor1DetailLevels = null;

        if (floor0Map != null) {
            floor0Tiles = ModelTranslation.realmListFromMapTileList(
                    ModelTranslation.getMapTilesFromFloorMap(floor0Code, floor0Map));
            floor0Resolutions = ModelTranslation.realmListFromDetailLevelResList(
                    ModelTranslation.getDetailLevelResFromFloorMap(floor0Code, floor0Map));
            floor0DetailLevels = ModelTranslation.realmFromDetailLevels(
                    new FloorDetailLevels(floor0Code, floor0Map.getLevels().size()));
        }
        if (floor1Map != null) {
            floor1Tiles = ModelTranslation.realmListFromMapTileList(
                    ModelTranslation.getMapTilesFromFloorMap(floor1Code, floor1Map));
            floor1Resolutions = ModelTranslation.realmListFromDetailLevelResList(
                    ModelTranslation.getDetailLevelResFromFloorMap(floor1Code, floor1Map));
            floor1DetailLevels = ModelTranslation.realmFromDetailLevels(
                    new FloorDetailLevels(floor1Code, floor1Map.getLevels().size()));
        }

        Realm r = open();

        try {
            beginTransaction(r);
            setMapResolutionsImpl(r, floor0Resolutions, floor1Resolutions);
            setMapsImpl(r, versionNum, floor0Tiles, floor1Tiles);
            setDetailLevelsImpl(r, floor0DetailLevels, floor1DetailLevels);
            commitTransaction(r);
        } catch (RuntimeException re) {
            cancelTransaction(r);
            re.printStackTrace();
            throw new InternalDatabaseError(re.toString());
        } finally {
            close(r);
        }
    }

    private Integer getDetailLevelsForFloorImpl(Realm realm, Integer floorNo) {
        return realm.where(FloorDetailLevelsRealm.class).equalTo("floorNo", floorNo).findFirst().getDetailLevels();
    }

    public Integer getDetailLevelsForFloor(Integer floorNo) {
        Integer detailLevels = null;
        Realm r = open();

        try {
            beginTransaction(r);
            detailLevels = getDetailLevelsForFloorImpl(r, floorNo);
            commitTransaction(r);
        } catch (RuntimeException re) {
            cancelTransaction(r);
            re.printStackTrace();
            throw new InternalDatabaseError(re.toString());
        } finally {
            close(r);
        }
        return detailLevels;
    }

    /*
     * returns exhibit with specific id if exists, otherwise null
     */
    private Exhibit getExhibitImpl(Realm realm, Integer id) {
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
    private void setExhibitImpl(Realm realm, Exhibit e) {
        ExhibitRealm er = ModelTranslation.realmFromExhibit(e);
        realm.copyToRealmOrUpdate(er);
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
    private List<Exhibit> getAllExhibitsImpl(Realm realm) {
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
    private void setAllExhibitsImpl(Realm realm, Iterable<Exhibit> allExhibits) {
        realm.clear(ExhibitRealm.class);

        for (Exhibit e : allExhibits) {
            realm.copyToRealmOrUpdate(ModelTranslation.realmFromExhibit(e));
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
    private List<Exhibit> getAllExhibitsForFloorImpl(Realm realm, Integer floor) {
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
    private void setExhibitsForFloorImpl(Realm realm, Iterable<Exhibit> exhibitsForFloor, Integer floor) {
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
    private void addOrUpdateExhibitsImpl(Realm realm, Iterable<Exhibit> exhibits) {
        realm.copyToRealmOrUpdate(ModelTranslation.realmFromExhibits(exhibits));
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

    private Integer getNextRaportIdImpl(Realm realm) {
        Number maxId = realm.where(RaportFileRealm.class).max("id");
        return maxId == null ? 0 : maxId.intValue() + 1;
    }

    public Integer getNextRaportId() {
        Integer result;
        Realm r = open();

        try {
            beginTransaction(r);
            result = getNextRaportIdImpl(r);
            commitTransaction(r);
        } catch (RuntimeException re) {
            cancelTransaction(r);
            re.printStackTrace();
            throw new InternalDatabaseError("Exception getting next raport id.");
        } finally {
            close(r);
        }
        return result;
    }

    private String getRaportFilenameImpl(Realm realm, Integer id) {
        RaportFileRealm result = realm.where(RaportFileRealm.class).equalTo("id", id).findFirst();
        return result == null ? null : result.getFileName();
    }

    public String getRaportFilename(Integer id) {
        String raportFile = null;
        Realm r = open();

        try {
            beginTransaction(r);
            raportFile = getRaportFilenameImpl(r, id);
            commitTransaction(r);
        } catch (RuntimeException re) {
            cancelTransaction(r);
            re.printStackTrace();
            throw new InternalDatabaseError(re.toString());
        } finally {
            close(r);
        }

        return raportFile;
    }

    private void setRaportFileImpl(Realm realm, Integer id, String raportFilename) {
        RaportFileRealm raportFileRealm = new RaportFileRealm();
        raportFileRealm.setId(id);
        raportFileRealm.setFileName(raportFilename);
        raportFileRealm.setState(RaportFileRealm.IN_PROGRESS);
        raportFileRealm.setServerId(null);
        realm.copyToRealmOrUpdate(raportFileRealm);
    }

    public void setRaportFile(Integer id, String raportFilename) {
        Realm r = open();

        try {
            beginTransaction(r);
            setRaportFileImpl(r, id, raportFilename);
            commitTransaction(r);
        } catch (Exception e) {
            cancelTransaction(r);
            e.printStackTrace();
            throw new InternalDatabaseError("Exception setting map of floor "
                    + id.toString() + " to " + raportFilename);
        } finally {
            close(r);
        }
    }

    private List<RaportFile> getAllReadyRaportsImpl(Realm realm) {
        List<RaportFile> raports = new ArrayList<>();
        RealmResults<RaportFileRealm> results = realm.where(RaportFileRealm.class).equalTo("state", RaportFileRealm.READY_TO_SEND).findAll();
        for (RaportFileRealm r : results) {
            raports.add(ModelTranslation.raportFileFromRealm(r));
        }
        return raports;
    }

    public List<RaportFile> getAllReadyRaports() {
        List<RaportFile> raports;
        Realm r = open();

        try {
            beginTransaction(r);
            raports = getAllReadyRaportsImpl(r);
            commitTransaction(r);
        } catch (RuntimeException re) {
            cancelTransaction(r);
            re.printStackTrace();
            throw new InternalDatabaseError(re.toString());
        } finally {
            close(r);
        }

        return raports;
    }

    private void changeRaportStateImpl(Realm realm, Integer id, String newState) {
        RaportFileRealm entry = realm.where(RaportFileRealm.class).equalTo("id", id).findFirst();
        entry.setState(newState);
        realm.copyToRealmOrUpdate(entry);
    }

    public void changeRaportState(Integer id, String newState) {
        Realm r = open();

        try {
            beginTransaction(r);
            changeRaportStateImpl(r, id, newState);
            commitTransaction(r);
        } catch (RuntimeException re) {
            cancelTransaction(r);
            re.printStackTrace();
            throw new InternalDatabaseError(re.toString());
        } finally {
            close(r);
        }
    }

    private void changeRaportServerIdImpl(Realm realm, Integer id, Integer serverId) {
        RaportFileRealm entry = realm.where(RaportFileRealm.class).equalTo("id", id).findFirst();
        entry.setServerId(serverId);
        realm.copyToRealmOrUpdate(entry);
    }

    public void changeRaportServerId(Integer id, Integer serverId) {
        Realm r =open();

        try {
            beginTransaction(r);
            changeRaportServerIdImpl(r, id, serverId);
            commitTransaction(r);
        } catch (RuntimeException re) {
            cancelTransaction(r);
            re.printStackTrace();
            throw new InternalDatabaseError(re.toString());
        } finally {
            close(r);
        }
    }
}
