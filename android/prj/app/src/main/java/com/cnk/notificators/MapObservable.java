package com.cnk.notificators;

import com.cnk.notificators.actions.MapUpdateAction;

import java.util.HashMap;
import java.util.Map;

public class MapObservable {
    protected Map<Observer, MapUpdateAction> observers;

    public MapObservable() {
        observers = new HashMap<>();
    }

    public void addObserver(Observer o, MapUpdateAction action) {
        observers.put(o, action);
    }

    public void deleteObserver(Observer o) {
        observers.remove(o);
    }

    public void notifyObservers() {
        for (Map.Entry<Observer, MapUpdateAction> entry : observers.entrySet()) {
            entry.getValue().doOnUpdate();
        }
    }
}
