package com.cnk.database;

import com.cnk.data.FloorMap;
import com.cnk.data.MapTiles;
import com.cnk.data.Resolution;
import com.cnk.database.models.DetailLevelRes;
import com.cnk.database.models.Exhibit;
import com.cnk.database.models.FloorDetailLevels;
import com.cnk.database.models.FloorTileSize;
import com.cnk.database.models.MapTile;
import com.cnk.database.models.RaportFile;
import com.cnk.database.models.Version;
import com.cnk.database.realm.DetailLevelResRealm;
import com.cnk.database.realm.ExhibitRealm;
import com.cnk.database.realm.FloorDetailLevelsRealm;
import com.cnk.database.realm.FloorTileSizeRealm;
import com.cnk.database.realm.MapTileRealm;
import com.cnk.database.realm.RaportFileRealm;
import com.cnk.database.realm.VersionRealm;

import java.util.ArrayList;
import java.util.List;

public class ModelTranslation {
    public static Version versionFromRealm(VersionRealm vr) {
        Version v = null;
        if (vr != null) {
            v = new Version(Version.Item.fromString(vr.getItem()), vr.getCurrentVersion());
        }

        return v;
    }

    public static VersionRealm realmFromVersion(Version v) {
        assert(v != null);

        VersionRealm vr = new VersionRealm();
        vr.setItem(v.getItem().toString());
        vr.setCurrentVersion(v.getCurrentVersion());

        return vr;
    }

    public static Exhibit exhibitFromRealm(ExhibitRealm er) {
        Exhibit e = null;
        if (er != null) {
            e = new Exhibit(er.getId(), er.getX(), er.getY(), er.getWidth(),
                    er.getHeight(), er.getFloor(), er.getName());
        }

        return e;
    }

    public static ExhibitRealm realmFromExhibit(Exhibit e) {
        assert (e != null);

        ExhibitRealm er = new ExhibitRealm();
        er.setId(e.getId());
        er.setX(e.getX());
        er.setY(e.getY());
        er.setWidth(e.getWidth());
        er.setHeight(e.getHeight());
        er.setFloor(e.getFloor());
        er.setName(e.getName());

        return er;
    }

    public static ArrayList<ExhibitRealm> realmFromExhibits(Iterable<Exhibit> exhibits) {
        ArrayList<ExhibitRealm> list = new ArrayList<>();
        for (Exhibit e : exhibits) {
            list.add(realmFromExhibit(e));
        }

        return list;
    }

    public static RaportFileRealm realmFromRaportFile(RaportFile file) {
        assert(file != null);

        RaportFileRealm realmRaport = new RaportFileRealm();
        realmRaport.setFileName(file.getFileName());
        realmRaport.setId(file.getId());
        realmRaport.setServerId(file.getServerId());
        realmRaport.setState(file.getState());
        return realmRaport;
    }

    public static RaportFile raportFileFromRealm(RaportFileRealm realmFile) {
        RaportFile file = null;
        if (realmFile != null) {
            file = new RaportFile(realmFile.getId(), realmFile.getServerId(),
                       realmFile.getFileName(), realmFile.getState());
        }

        return file;
    }

    public static ArrayList<MapTile> getMapTilesFromFloorMap(Integer floor, FloorMap fm) {
        ArrayList<MapTile> result = new ArrayList<>();

        List<MapTiles> detailLevels = fm.getLevels();

        Integer detailLevel = 0;
        Integer rowNum;
        Integer columnNum;
        for (MapTiles mt : detailLevels) {
            rowNum = 0;
            ArrayList<ArrayList<String>> tilesForLevel = mt.getTilesFiles();
            for (List<String> row : tilesForLevel) {
                columnNum = 0;
                for (String location : row) {
                    result.add(new MapTile(floor, detailLevel, rowNum, columnNum, location));

                    columnNum++;
                }
                rowNum++;
            }
            detailLevel++;
        }

        return result;
    }

    public static List<DetailLevelRes> getDetailLevelResFromFloorMap(Integer floor, FloorMap fm) {
        ArrayList<DetailLevelRes> result = new ArrayList<>();

        List<MapTiles> detailLevels = fm.getLevels();
        Integer level = 0;
        for (MapTiles mt : detailLevels) {
            result.add(new DetailLevelRes(floor, level, fm.getOriginalSize(), mt.getScaledSize()));
            level++;
        }

        return result;
    }

    public static MapTileRealm realmFromMapTile(MapTile mt) {
        assert(mt != null);

        MapTileRealm mtr = new MapTileRealm();
        mtr.setFloor(mt.getFloor());
        mtr.setDetailLevel(mt.getDetailLevel());
        mtr.setRowNumber(mt.getRowNumber());
        mtr.setColumnNumber(mt.getColumnNumber());
        mtr.setMapTileLocation(mt.getMapTileLocation());

        return mtr;
    }

    public static MapTile mapTileFromRealm(MapTileRealm mtr) {
        if (mtr == null) {
            return null;
        }

        return new MapTile(mtr.getFloor(), mtr.getDetailLevel(), mtr.getRowNumber(),
                mtr.getColumnNumber(), mtr.getMapTileLocation());
    }

    public static List<MapTileRealm> realmListFromMapTileList(List<MapTile> mtl) {
        assert(mtl != null);

        ArrayList<MapTileRealm> mtrl = new ArrayList<>();

        for (MapTile mt : mtl) {
            mtrl.add(realmFromMapTile(mt));
        }

        return mtrl;
    }

    public static List<MapTile> mapTileListFromRealmList(List<MapTileRealm> mtrl) {
        if (mtrl == null) {
            return null;
        }

        ArrayList<MapTile> mtl = new ArrayList<>();

        for (MapTileRealm mtr : mtrl) {
            mtl.add(mapTileFromRealm(mtr));
        }

        return mtl;
    }

    public static DetailLevelResRealm realmFromDetailLevelRes(DetailLevelRes dlr) {
        assert(dlr != null);

        DetailLevelResRealm dlrr = new DetailLevelResRealm();
        dlrr.setFloor(dlr.getFloor());
        dlrr.setDetailLevel(dlr.getDetailLevel());
        dlrr.setOriginalWidth(dlr.getOriginalRes().getWidth());
        dlrr.setOriginalHeight(dlr.getOriginalRes().getHeight());
        dlrr.setScaledWidth(dlr.getScaledRes().getWidth());
        dlrr.setScaledHeight(dlr.getScaledRes().getHeight());

        return dlrr;
    }

    public static DetailLevelRes detailLevelResFromRealm(DetailLevelResRealm dlrr) {
        if (dlrr == null) {
            return null;
        }

        return new DetailLevelRes(dlrr.getFloor(), dlrr.getDetailLevel(),
                new Resolution(dlrr.getOriginalWidth(), dlrr.getOriginalHeight()),
                new Resolution(dlrr.getScaledWidth(), dlrr.getScaledHeight()));
    }

    public static List<DetailLevelResRealm> realmListFromDetailLevelResList(List<DetailLevelRes> dlrl) {
        assert(dlrl != null);

        ArrayList<DetailLevelResRealm> dlrrl = new ArrayList<>();

        for (DetailLevelRes dlr : dlrl) {
            dlrrl.add(realmFromDetailLevelRes(dlr));
        }

        return dlrrl;
    }

    public static List<DetailLevelRes> detailLevelResListFromRealmList(List<DetailLevelResRealm> dlrrl) {
        if (dlrrl == null) {
            return null;
        }

        ArrayList<DetailLevelRes> dlrl = new ArrayList<>();

        for (DetailLevelResRealm dlrr : dlrrl) {
            dlrl.add(detailLevelResFromRealm(dlrr));
        }

        return dlrl;
    }

    public static FloorDetailLevels detailLevelsFromRealm(FloorDetailLevelsRealm fdlr) {
        if (fdlr == null) {
            return null;
        }

        return new FloorDetailLevels(fdlr.getFloorNo(), fdlr.getDetailLevels());
    }

    public static FloorDetailLevelsRealm realmFromDetailLevels(FloorDetailLevels floorDetails) {
        assert(floorDetails != null);

        FloorDetailLevelsRealm floorDetailRealm = new FloorDetailLevelsRealm();
        floorDetailRealm.setFloorNo(floorDetails.getFloorNo());
        floorDetailRealm.setDetailLevels(floorDetails.getDetailLevels());
        return floorDetailRealm;
    }

    public static List<FloorTileSizeRealm> getTileSizeForDetailLevel(FloorMap map, Integer floorNo) {
        ArrayList<FloorTileSizeRealm> res = new ArrayList<>();
        for (int i = 0; i < map.getLevels().size(); i++) {
            FloorTileSizeRealm current = new FloorTileSizeRealm();
            current.setFloor(floorNo);
            current.setDetailLevel(i);
            current.setWidth(map.getLevels().get(i).getTileSize().getWidth());
            current.setLength(map.getLevels().get(i).getTileSize().getHeight());
            res.add(current);
        }
        return res;
    }

    public static FloorTileSize tileSizeFromRealm(FloorTileSizeRealm realm) {
        if (realm == null) {
            return null;
        }
        return new FloorTileSize(realm.getFloor(), realm.getDetailLevel(),
                                 new Resolution(realm.getWidth(), realm.getLength()));
    }
}
