package com.cnk.data.raport;

import java.io.Serializable;
import java.util.ArrayList;
import java.util.List;

public class Raport implements Serializable {
    private Integer id;
    private List<RaportEvent> history;

    public Raport(Integer id) {
        this.id = id;
        history = new ArrayList<>();
    }

    public void addEvent(RaportEvent event) {
        history.add(event);
    }

    public List<RaportEvent> getHistory() {
        return history;
    }

    public Integer getId() {
        return id;
    }

    public void setId(Integer id) {
        this.id = id;
    }
}
