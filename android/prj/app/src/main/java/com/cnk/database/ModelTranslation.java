package com.cnk.database;

import android.graphics.Color;

import com.cnk.data.map.FloorMap;
import com.cnk.data.map.MapTiles;
import com.cnk.data.map.Resolution;
import com.cnk.database.models.DetailLevelRes;
import com.cnk.database.models.Exhibit;
import com.cnk.database.models.FloorDetailLevels;
import com.cnk.database.models.MapTileInfo;
import com.cnk.database.models.RaportFile;
import com.cnk.database.realm.DetailLevelResRealm;
import com.cnk.database.realm.ExhibitRealm;
import com.cnk.database.realm.FloorDetailLevelsRealm;
import com.cnk.database.realm.MapTileInfoRealm;
import com.cnk.database.realm.RaportFileRealm;

import java.util.ArrayList;
import java.util.List;

public class ModelTranslation {

    public static Exhibit exhibitFromRealm(ExhibitRealm er) {
        Exhibit e = null;
        if (er != null) {
            e =
                    new Exhibit(er.getId(),
                                er.getX(),
                                er.getY(),
                                er.getWidth(),
                                er.getHeight(),
                                er.getFloor(),
                                Color.rgb(er.getColorR(), er.getColorG(), er.getColorB()),
                                er.getName());
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
        if (e.getColor() != null) {
            er.setColorR(Color.red(e.getColor()));
            er.setColorG(Color.green(e.getColor()));
            er.setColorB(Color.blue(e.getColor()));
        }
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

    public static RaportFile raportFileFromRealm(RaportFileRealm realmFile) {
        RaportFile file = null;
        if (realmFile != null) {
            file =
                    new RaportFile(realmFile.getId(),
                                   realmFile.getServerId(),
                                   realmFile.getFileName(),
                                   realmFile.getState());
        }

        return file;
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

    public static DetailLevelResRealm realmFromDetailLevelRes(DetailLevelRes dlr) {
        assert (dlr != null);

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

        return new DetailLevelRes(dlrr.getFloor(),
                                  dlrr.getDetailLevel(),
                                  new Resolution(dlrr.getOriginalWidth(), dlrr.getOriginalHeight()),
                                  new Resolution(dlrr.getScaledWidth(), dlrr.getScaledHeight()));
    }

    public static List<DetailLevelResRealm> realmListFromDetailLevelResList(List<DetailLevelRes> dlrl) {
        assert (dlrl != null);

        ArrayList<DetailLevelResRealm> dlrrl = new ArrayList<>();

        for (DetailLevelRes dlr : dlrl) {
            dlrrl.add(realmFromDetailLevelRes(dlr));
        }

        return dlrrl;
    }

    public static FloorDetailLevelsRealm realmFromDetailLevels(FloorDetailLevels floorDetails) {
        assert (floorDetails != null);

        FloorDetailLevelsRealm floorDetailRealm = new FloorDetailLevelsRealm();
        floorDetailRealm.setFloorNo(floorDetails.getFloorNo());
        floorDetailRealm.setDetailLevels(floorDetails.getDetailLevels());
        return floorDetailRealm;
    }

    public static List<MapTileInfoRealm> getMapTileInfo(FloorMap map, Integer floorNo) {
        ArrayList<MapTileInfoRealm> res = new ArrayList<>();
        for (int i = 0; i < map.getLevels().size(); i++) {
            MapTileInfoRealm current = new MapTileInfoRealm();
            current.setFloor(floorNo);
            current.setDetailLevel(i);
            current.setWidth(map.getLevels().get(i).getTileSize().getWidth());
            current.setLength(map.getLevels().get(i).getTileSize().getHeight());
            res.add(current);
        }
        return res;
    }

    public static MapTileInfo mapTileInfoFromRealm(MapTileInfoRealm realm) {
        if (realm == null) {
            return null;
        }
        return new MapTileInfo(realm.getFloor(),
                               realm.getDetailLevel(),
                               new Resolution(realm.getWidth(), realm.getLength()));
    }

}
