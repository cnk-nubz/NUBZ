package com.cnk.notificators;


import com.cnk.communication.task.ServerTask;

import java.util.Observable;
import java.util.Observer;

public class Notificator extends Observable {

    public Notificator(Observer o) {
        this.addObserver(o);
    }

    public void failure(ServerTask task) {
        setChanged();
        notifyObservers(false);
    }

    public void success() {
        setChanged();
        notifyObservers(true);
    }


}
