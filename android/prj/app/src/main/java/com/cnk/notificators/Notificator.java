package com.cnk.notificators;


import java.util.Observable;

public class Notificator extends Observable {

    public void failure() {
        setChanged();
        notifyObservers();
    }


}
