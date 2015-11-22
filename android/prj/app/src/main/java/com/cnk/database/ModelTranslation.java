package com.cnk.database;

public class ModelTranslation {
    public static MapFile mapFileFromRealm(MapFileRealm mfr) {
        MapFile mf = new MapFile();
        mf.setFloor(mfr.getFloor());
        mf.setMapFileLocation(mfr.getMapFileLocation());

        return mf;
    }

    public static MapFileRealm realmFromMapFile(MapFile mf) {
        MapFileRealm mfr = new MapFileRealm();
        mfr.setFloor(mf.getFloor());
        mfr.setMapFileLocation(mf.getMapFileLocation());

        return mfr;
    }

    public static Version versionFromRealm(VersionRealm vr) {
        Version v = new Version();
        v.setItem(Version.Item.fromString(vr.getItem()));
        v.setCurrentVersion(vr.getCurrentVersion());

        return v;
    }

    public static VersionRealm realmFromVerion(Version v) {
        VersionRealm vr = new VersionRealm();
        vr.setItem(v.getItem().toString());
        vr.setCurrentVersion(v.getCurrentVersion());

        return vr;
    }
}
