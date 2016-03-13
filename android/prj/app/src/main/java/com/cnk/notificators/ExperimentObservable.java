package com.cnk.notificators;

import com.cnk.notificators.actions.ExperimentUpdateAction;

import java.util.HashMap;
import java.util.Map;

public class ExperimentObservable {
    protected Map<Observer, ExperimentUpdateAction> observers;

    public ExperimentObservable() {
        observers = new HashMap<>();
    }

    public void addObserver(Observer o, ExperimentUpdateAction action) {
        observers.put(o, action);
    }

    public void deleteObserver(Observer o) {
        observers.remove(o);
    }

    public void notifyObservers() {
        for (Map.Entry<Observer, ExperimentUpdateAction> entry : observers.entrySet()) {
            entry.getValue().doOnUpdate();
        }
    }
}
