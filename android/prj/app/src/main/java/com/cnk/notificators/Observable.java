package com.cnk.notificators;

import java.util.Map;
import java.util.WeakHashMap;

public class Observable<T> {
    protected Map<Observer, T> observers;

    public Observable() {
        observers = new WeakHashMap<>();
    }

    public void addObserver(Observer o, T action) {
        observers.put(o, action);
    }

    public void deleteObserver(Observer o) {
        observers.remove(o);
    }
}
