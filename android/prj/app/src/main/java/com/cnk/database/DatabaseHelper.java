package com.cnk.database;

import android.content.Context;

import com.cnk.data.map.FloorMap;
import com.cnk.database.models.DetailLevelRes;
import com.cnk.database.models.Exhibit;
import com.cnk.database.models.FloorDetailLevels;
import com.cnk.database.models.MapTileInfo;
import com.cnk.database.models.RaportFile;
import com.cnk.database.models.Version;
import com.cnk.database.realm.DetailLevelResRealm;
import com.cnk.database.realm.ExhibitRealm;
import com.cnk.database.realm.FloorDetailLevelsRealm;
import com.cnk.database.realm.MapTileInfoRealm;
import com.cnk.database.realm.RaportFileRealm;
import com.cnk.database.realm.VersionRealm;
import com.cnk.exceptions.InternalDatabaseError;
import com.cnk.utilities.Consts;

import java.util.ArrayList;
import java.util.List;

import io.realm.Realm;
import io.realm.RealmResults;

public class DatabaseHelper {
    Context applicationContext;

    private class Value<T> {
        public T val;
    }

    private interface DatabaseAction {
        void perform(Realm r);
    }

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

    private void inTransaction(DatabaseAction action) {
        Realm r = open();
        try {
            beginTransaction(r);
            action.perform(r);
            commitTransaction(r);
        } catch (RuntimeException re) {
            cancelTransaction(r);
            re.printStackTrace();
            throw new InternalDatabaseError(re.toString());
        } finally {
            close(r);
        }
    }

    public Integer getVersion(Version.Item item) {
        Value<Integer> versionNumber = new Value<>();
        inTransaction((realm) -> {
            VersionRealm
                    result =
                    realm.where(VersionRealm.class).equalTo("item", item.toString()).findFirst();
            versionNumber.val = result == null ? null : result.getCurrentVersion();
        });
        return versionNumber.val;
    }

    private void setVersion(Realm realm, Version.Item item, Integer versionNumber) {
        VersionRealm version = new VersionRealm();
        version.setItem(item.toString());
        version.setCurrentVersion(versionNumber);
        realm.copyToRealmOrUpdate(version);
    }

    public DetailLevelRes getDetailLevelRes(Integer floor, Integer detailLevel) {
        Value<DetailLevelRes> res = new Value<>();
        inTransaction((realm) -> {
            DetailLevelResRealm
                    dbRes =
                    realm.where(DetailLevelResRealm.class)
                         .equalTo("floor", floor)
                         .equalTo("detailLevel", detailLevel)
                         .findFirst();
            res.val = ModelTranslation.detailLevelResFromRealm(dbRes);
        });
        return res.val;
    }

    public void setMaps(Integer versionNum, List<FloorMap> floorMaps) {
        List<List<DetailLevelResRealm>> floorsResolutions = new ArrayList<>();
        List<FloorDetailLevelsRealm> floorsDetailLevels = new ArrayList<>();
        List<List<MapTileInfoRealm>> floorsTileInfos = new ArrayList<>();
        for (int currentFloorNo = 0; currentFloorNo < Consts.FLOOR_COUNT; currentFloorNo++) {
            FloorMap currentFloor = floorMaps.get(currentFloorNo);
            if (currentFloor == null) {
                floorsResolutions.add(null);
                floorsDetailLevels.add(null);
                floorsTileInfos.add(null);
                continue;
            }
            List<DetailLevelRes>
                    resolutions =
                    ModelTranslation.getDetailLevelResFromFloorMap(currentFloorNo, currentFloor);
            floorsResolutions.add(ModelTranslation.realmListFromDetailLevelResList(resolutions));
            FloorDetailLevels
                    detailLevels =
                    new FloorDetailLevels(currentFloorNo, currentFloor.getLevels().size());
            floorsDetailLevels.add(ModelTranslation.realmFromDetailLevels(detailLevels));
            floorsTileInfos.add(ModelTranslation.getMapTileInfo(currentFloor, currentFloorNo));
        }

        inTransaction((realm) -> {
            setVersion(realm, Version.Item.MAP, versionNum);
            setMapsResolution(realm, floorsResolutions);
            setDetailLevels(realm, floorsDetailLevels);
            setMapTilesInfos(realm, floorsTileInfos);
        });
    }

    private void setMapsResolution(Realm realm, List<List<DetailLevelResRealm>> floorsResolutions) {
        for (int currentFloorNo = 0; currentFloorNo < Consts.FLOOR_COUNT; currentFloorNo++) {
            realm.where(DetailLevelResRealm.class)
                 .equalTo("floor", currentFloorNo)
                 .findAll()
                 .clear();
            realm.copyToRealm(floorsResolutions.get(currentFloorNo));
        }
    }

    private void setDetailLevels(Realm realm, List<FloorDetailLevelsRealm> floorsDetailLevels) {
        for (int currentFLoorNo = 0; currentFLoorNo < Consts.FLOOR_COUNT; currentFLoorNo++) {
            realm.where(FloorDetailLevelsRealm.class)
                 .equalTo("floorNo", floorsDetailLevels.get(currentFLoorNo).getFloorNo())
                 .findAll()
                 .clear();
            realm.copyToRealm(floorsDetailLevels.get(currentFLoorNo));
        }
    }

    private void setMapTilesInfos(Realm realm, List<List<MapTileInfoRealm>> floorsInfos) {
        for (int currentFloorNo = 0; currentFloorNo < Consts.FLOOR_COUNT; currentFloorNo++) {
            List<MapTileInfoRealm> floorInfo = floorsInfos.get(currentFloorNo);
            for (MapTileInfoRealm tileInfo : floorInfo) {
                realm.where(MapTileInfoRealm.class)
                     .equalTo("floor", tileInfo.getFloor())
                     .equalTo("detailLevel", tileInfo.getDetailLevel())
                     .findAll()
                     .clear();
                realm.copyToRealm(tileInfo);
            }
        }
    }

    public Integer getDetailLevelsForFloor(Integer floorNo) {
        Value<Integer> detailLevels = new Value<>();
        inTransaction((realm) -> {
            FloorDetailLevelsRealm
                    res =
                    realm.where(FloorDetailLevelsRealm.class)
                         .equalTo("floorNo", floorNo)
                         .findFirst();
            detailLevels.val = res == null ? null : res.getDetailLevels();
        });
        return detailLevels.val;
    }

    public MapTileInfo getMapTileInfo(Integer floorNo, Integer detailLevel) {
        Value<MapTileInfo> res = new Value<>();
        inTransaction((realm) -> {
            MapTileInfoRealm
                    dbRes =
                    realm.where(MapTileInfoRealm.class)
                         .equalTo("floor", floorNo)
                         .equalTo("detailLevel", detailLevel)
                         .findFirst();
            res.val = ModelTranslation.mapTileInfoFromRealm(dbRes);
        });
        return res.val;
    }

    public List<Exhibit> getAllExhibitsForFloor(Integer floor) {
        List<Exhibit> exhibits = new ArrayList<>();
        inTransaction((realm) -> {
            RealmResults<ExhibitRealm>
                    results =
                    realm.where(ExhibitRealm.class).equalTo("floor", floor).findAll();

            for (ExhibitRealm er : results) {
                exhibits.add(ModelTranslation.exhibitFromRealm(er));
            }
        });
        return exhibits;
    }

    public void addOrUpdateExhibits(Integer versionNum, Iterable<Exhibit> exhibits) {
        inTransaction((realm) -> {
            realm.copyToRealmOrUpdate(ModelTranslation.realmFromExhibits(exhibits));
            setVersion(realm, Version.Item.EXHIBITS, versionNum);
        });
    }

    public Integer getNextRaportId() {
        Value<Number> maxId = new Value<>();
        inTransaction((realm) -> {
            maxId.val = realm.where(RaportFileRealm.class).max("id");
        });
        return maxId.val == null ? 0 : maxId.val.intValue() + 1;
    }

    public void setRaportFile(Integer id, String raportFilename) {
        inTransaction((realm) -> {
            RaportFileRealm raportFileRealm = new RaportFileRealm();
            raportFileRealm.setId(id);
            raportFileRealm.setFileName(raportFilename);
            raportFileRealm.setState(RaportFileRealm.IN_PROGRESS);
            raportFileRealm.setServerId(null);
            realm.copyToRealmOrUpdate(raportFileRealm);
        });
    }

    public List<RaportFile> getAllReadyRaports() {
        List<RaportFile> raports = new ArrayList<>();
        inTransaction((realm) -> {
            RealmResults<RaportFileRealm>
                    results =
                    realm.where(RaportFileRealm.class)
                         .equalTo("state", RaportFileRealm.READY_TO_SEND)
                         .findAll();
            for (RaportFileRealm r : results) {
                raports.add(ModelTranslation.raportFileFromRealm(r));
            }
        });
        return raports;
    }

    public void changeRaportState(Integer id, String newState) {
        inTransaction((realm) -> {
            RaportFileRealm
                    entry =
                    realm.where(RaportFileRealm.class).equalTo("id", id).findFirst();
            entry.setState(newState);
            realm.copyToRealmOrUpdate(entry);
        });
    }

    public void changeRaportServerId(Integer id, Integer serverId) {
        inTransaction((realm) -> {
            RaportFileRealm
                    entry =
                    realm.where(RaportFileRealm.class).equalTo("id", id).findFirst();
            entry.setServerId(serverId);
            realm.copyToRealmOrUpdate(entry);
        });
    }
}
