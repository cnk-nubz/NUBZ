package com.cnk.data.exhibits;

import com.cnk.database.models.Exhibit;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class FloorExhibitsInfo {
    private Lock lock;
    private Map<Integer, Exhibit> exhibits;

    public FloorExhibitsInfo() {
        exhibits = new HashMap<>();
        lock = new ReentrantLock(true);
    }

    public void removeExhibit(Integer id) {
        lock.lock();
        exhibits.remove(id);
        lock.unlock();
    }

    public List<Exhibit> getExhibitsList() {
        lock.lock();
        List<Exhibit> ret = new ArrayList<>(exhibits.values());
        lock.unlock();
        return ret;
    }

    public void addExhibit(Exhibit e) {
        lock.lock();
        exhibits.put(e.getId(), e);
        lock.unlock();
    }

    public void setExhibits(Map<Integer, Exhibit> exhibits) {
        lock.lock();
        this.exhibits = exhibits;
        lock.unlock();
    }
}
