package com.cnk.notificators;

import android.app.Activity;

import java.util.Map;
import java.util.WeakHashMap;

public class Observable<T> {
    protected Map<Observer, T> observers;
    protected Map<Activity, T> uiObservers;

    public Observable() {
        observers = new WeakHashMap<>();
        uiObservers = new WeakHashMap<>();
    }

    public void addObserver(Observer o, T action) {
        observers.put(o, action);
    }

    public void addUIObserver(Activity o, T action) {
        uiObservers.put(o, action);
    }

    public void deleteObserver(Observer o) {
        observers.remove(o);
        uiObservers.remove(o);
    }
}
