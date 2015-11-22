package com.cnk.notificators;


import java.util.Observable;
import java.util.Observer;

public class Notificator extends Observable {

    public Notificator(Observer o) {
        this.addObserver(o);
    }

    public void failure() {
        setChanged();
        notifyObservers(false);
    }

    public void success() {
        setChanged();
        notifyObservers(true);
    }


}
