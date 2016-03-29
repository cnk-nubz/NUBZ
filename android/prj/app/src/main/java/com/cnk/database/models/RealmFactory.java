package com.cnk.database.models;

import java.util.ArrayList;

import io.realm.RealmObject;

public class RealmFactory<T extends Realmable> {
    private static RealmFactory instance;

    private RealmFactory() {
    }

    public static RealmFactory getInstance() {
        if (instance == null) {
            instance = new RealmFactory();
        }
        return instance;
    }

    public ArrayList<RealmObject> toRealmList(Iterable<T> raw) {
        ArrayList<RealmObject> list = new ArrayList<>();
        for (T r : raw) {
            list.add(r.toRealm());
        }
        return list;
    }
}
