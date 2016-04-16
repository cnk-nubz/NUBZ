package com.cnk.database;

import android.content.Context;

import com.cnk.data.map.FloorMap;
import com.cnk.data.map.ZoomLevel;
import com.cnk.database.models.Exhibit;
import com.cnk.database.models.MapTileInfo;
import com.cnk.database.models.RaportFile;
import com.cnk.database.models.RealmFactory;
import com.cnk.database.models.Version;
import com.cnk.database.models.ZoomLevelResolution;
import com.cnk.database.realm.ExhibitRealm;
import com.cnk.database.realm.MapTileInfoRealm;
import com.cnk.database.realm.RaportFileRealm;
import com.cnk.database.realm.VersionRealm;
import com.cnk.database.realm.ZoomLevelResolutionRealm;
import com.cnk.exceptions.InternalDatabaseError;

import java.util.ArrayList;
import java.util.List;

import io.realm.Realm;
import io.realm.RealmResults;

public class DatabaseHelper {
    private interface DatabaseAction {
        void perform(Realm r);
    }

    private class Value<T> {
        public T val;
    }

    private Context applicationContext;

    public DatabaseHelper(Context applicationContext) {
        this.applicationContext = applicationContext;
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

    public Integer getVersion(Version item) {
        Value<Integer> versionNumber = new Value<>();
        inTransaction((realm) -> {
            VersionRealm result =
                    realm.where(VersionRealm.class).equalTo("item", item.toString()).findFirst();
            versionNumber.val = result == null ? null : result.getCurrentVersion();
        });
        return versionNumber.val;
    }

    private void setVersion(Realm realm, Version ver, Integer versionNumber) {
        VersionRealm version = new VersionRealm();
        version.setItem(ver.toString());
        version.setCurrentVersion(versionNumber);
        realm.copyToRealmOrUpdate(version);
    }

    public ZoomLevelResolution getZoomLevelResolution(Integer floor, Integer zoomLevel) {
        Value<ZoomLevelResolution> res = new Value<>();
        inTransaction((realm) -> {
            ZoomLevelResolutionRealm dbRes = realm.where(ZoomLevelResolutionRealm.class)
                                                  .equalTo("floor", floor)
                                                  .equalTo("zoomLevel", zoomLevel)
                                                  .findFirst();
            if (dbRes != null) {
                res.val = new ZoomLevelResolution(dbRes);
            }
        });
        return res.val;
    }

    public void setMap(FloorMap map) {
        List<ZoomLevelResolution> resolutions = new ArrayList<>();
        List<MapTileInfo> tilesInfos = new ArrayList<>();

        for (int i = 0; i < map.getZoomLevels().size(); i++) {
            ZoomLevel zoomLevel = map.getZoomLevels().get(i);

            ZoomLevelResolution res = new ZoomLevelResolution(map.getFloor(),
                                                              i,
                                                              map.getOriginalSize(),
                                                              zoomLevel.getScaledSize());
            resolutions.add(res);

            MapTileInfo tilesInfo = new MapTileInfo(map.getFloor(), i, zoomLevel.getTileSize());
            tilesInfos.add(tilesInfo);
        }

        inTransaction((realm) -> {
            setMapsResolution(realm, map.getFloor(), resolutions);
            setMapTilesInfos(realm, map.getFloor(), tilesInfos);
        });
    }

    private void setMapsResolution(Realm realm,
                                   Integer floor,
                                   List<ZoomLevelResolution> resolutions) {
        realm.where(ZoomLevelResolutionRealm.class).equalTo("floor", floor).findAll().clear();
        realm.copyToRealm(RealmFactory.getInstance().toRealmList(resolutions));
    }

    private void setMapTilesInfos(Realm realm, Integer floor, List<MapTileInfo> tilesInfos) {
        realm.where(MapTileInfoRealm.class).equalTo("floor", floor).findAll().clear();
        realm.copyToRealm(RealmFactory.getInstance().toRealmList(tilesInfos));
    }

    public int getZoomLevelsCount(Integer floor) {
        Value<Integer> res = new Value<>();
        inTransaction((realm) -> res.val = (int) realm.where(ZoomLevelResolutionRealm.class)
                                                      .equalTo("floor", floor)
                                                      .count());
        return res.val;
    }

    public MapTileInfo getMapTileInfo(Integer floorNo, Integer zoomLevel) {
        Value<MapTileInfo> res = new Value<>();
        inTransaction((realm) -> {
            MapTileInfoRealm dbRes = realm.where(MapTileInfoRealm.class)
                                          .equalTo("floor", floorNo)
                                          .equalTo("zoomLevel", zoomLevel)
                                          .findFirst();
            if (dbRes != null) {
                res.val = new MapTileInfo(dbRes);
            }
        });
        return res.val;
    }

    public List<Exhibit> getAllExhibitsForFloor(Integer floor) {
        List<Exhibit> exhibits = new ArrayList<>();
        inTransaction((realm) -> {
            RealmResults<ExhibitRealm> results =
                    realm.where(ExhibitRealm.class).equalTo("floor", floor).findAll();

            for (ExhibitRealm er : results) {
                exhibits.add(new Exhibit(er));
            }
        });
        return exhibits;
    }

    public void addOrUpdateExhibits(Integer versionNum, Iterable<Exhibit> exhibits) {
        inTransaction((realm) -> {
            realm.copyToRealmOrUpdate(RealmFactory.getInstance().toRealmList(exhibits));
            setVersion(realm, Version.EXHIBITS, versionNum);
        });
    }

    public Integer getNextRaportId() {
        Value<Number> maxId = new Value<>();
        inTransaction((realm) -> maxId.val = realm.where(RaportFileRealm.class).max("id"));
        return maxId.val == null ? 0 : maxId.val.intValue() + 1;
    }

    public void setRaportFile(Integer id, String raportFilename) {
        RaportFileRealm raportFileRealm = new RaportFileRealm();
        raportFileRealm.setId(id);
        raportFileRealm.setFileName(raportFilename);
        raportFileRealm.setState(RaportFileRealm.IN_PROGRESS);
        raportFileRealm.setServerId(null);
        inTransaction((realm) -> realm.copyToRealmOrUpdate(raportFileRealm));
    }

    public List<RaportFile> getAllReadyRaports() {
        List<RaportFile> raports = new ArrayList<>();
        inTransaction((realm) -> {
            RealmResults<RaportFileRealm> results = realm.where(RaportFileRealm.class)
                                                         .equalTo("state",
                                                                  RaportFileRealm.READY_TO_SEND)
                                                         .findAll();
            for (RaportFileRealm r : results) {
                raports.add(new RaportFile(r));
            }
        });
        return raports;
    }

    public void changeRaportState(Integer id, String newState) {
        inTransaction((realm) -> {
            RaportFileRealm entry =
                    realm.where(RaportFileRealm.class).equalTo("id", id).findFirst();
            entry.setState(newState);
            realm.copyToRealmOrUpdate(entry);
        });
    }

    public void changeRaportServerId(Integer id, Integer serverId) {
        inTransaction((realm) -> {
            RaportFileRealm entry =
                    realm.where(RaportFileRealm.class).equalTo("id", id).findFirst();
            entry.setServerId(serverId);
            realm.copyToRealmOrUpdate(entry);
        });
    }
}
