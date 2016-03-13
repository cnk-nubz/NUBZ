package com.cnk.data.experiment.raport;

import java.io.Serializable;
import java.util.ArrayList;
import java.util.List;

public class RaportEvent implements Serializable {
    private Integer exhibitId;
    private Integer durationSeconds;
    private List<Integer> actions;

    public RaportEvent(Integer exhibitId, Integer durationSeconds, List<Integer> actions) {
        this.exhibitId = exhibitId;
        this.durationSeconds = durationSeconds;
        this.actions = actions;
    }

    public Integer getExhibitId() {
        return exhibitId;
    }

    public Integer getDurationSeconds() {
        return durationSeconds;
    }

    public List<Integer> getActions() {
        return actions;
    }

    @Override
    public String toString() {
        return "RaportEvent{" +
               "exhibitId=" + exhibitId +
               ", durationSeconds=" + durationSeconds +
               ", actions=" + actions +
               '}';
    }
}
