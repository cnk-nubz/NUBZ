package com.cnk.data.raport;

import java.io.Serializable;
import java.util.ArrayList;
import java.util.List;

public class RaportEvent implements Serializable {
    private Integer exhibitId;
    private Integer durationSeconds;
    private List<Integer> actions;

    public RaportEvent(Integer exhibitId) {
        this.exhibitId = exhibitId;
        actions = new ArrayList<>();
    }

    public RaportEvent(Integer exhibitId, Integer durationSeconds, List<Integer> actions) {
        this.exhibitId = exhibitId;
        this.durationSeconds = durationSeconds;
        this.actions = actions;
    }

    public Integer getExhibitId() {
        return exhibitId;
    }

    public void setExhibitId(Integer exhibitId) {
        this.exhibitId = exhibitId;
    }

    public Integer getDurationSeconds() {
        return durationSeconds;
    }

    public void setDurationSeconds(Integer durationSeconds) {
        this.durationSeconds = durationSeconds;
    }

    public List<Integer> getActions() {
        return actions;
    }

    public void setActions(List<Integer> actions) {
        this.actions = actions;
    }

    public void addAction(Integer action) {
        actions.add(action);
    }

}
