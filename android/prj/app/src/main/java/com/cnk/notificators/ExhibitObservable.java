package com.cnk.notificators;

import com.cnk.database.models.Exhibit;
import com.cnk.notificators.actions.ExhibitsUpdateAction;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class ExhibitObservable {
    protected Map<Observer, ExhibitsUpdateAction> observers;

    public ExhibitObservable() {
        observers = new HashMap<>();
    }

    public void addObserver(Observer o, ExhibitsUpdateAction action) {
        observers.put(o, action);
    }

    public void deleteObserver(Observer o) {
        observers.remove(o);
    }

    public void notifyObservers(List<Exhibit> changedExhibits) {
        for (Map.Entry<Observer, ExhibitsUpdateAction> entry : observers.entrySet()) {
            entry.getValue().doOnUpdate(changedExhibits);
        }
    }
}
