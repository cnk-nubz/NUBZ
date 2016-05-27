package com.cnk.database.models;

import io.realm.RealmObject;

public interface Realmable {
    RealmObject toRealm();
}
