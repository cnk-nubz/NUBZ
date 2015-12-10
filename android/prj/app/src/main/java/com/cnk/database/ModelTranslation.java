package com.cnk.database;

import java.util.ArrayList;

public class ModelTranslation {
    public static MapFile mapFileFromRealm(MapFileRealm mfr) {
        MapFile mf = null;
        if (mfr != null) {
            mf = new MapFile(mfr.getFloor(), mfr.getMapFileLocation());
        }

        return mf;
    }

    public static MapFileRealm realmFromMapFile(MapFile mf) {
        assert(mf != null);

        MapFileRealm mfr = new MapFileRealm();
        mfr.setFloor(mf.getFloor());
        mfr.setMapFileLocation(mf.getMapFileLocation());

        return mfr;
    }

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
}
