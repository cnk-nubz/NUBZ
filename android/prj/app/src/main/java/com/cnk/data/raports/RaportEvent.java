package com.cnk.data.raports;

import java.io.Serializable;
import java.util.Date;
import java.util.List;

public class RaportEvent implements Serializable {
    private Integer exhibitId;
    private int durationSeconds;
    private Timestamp startDate;
    private List<Integer> actions;

    public RaportEvent(Integer exhibitId,
                       int durationSeconds,
                       List<Integer> actions,
                       Timestamp startDate) {
        this.exhibitId = exhibitId;
        this.durationSeconds = durationSeconds;
        this.actions = actions;
        this.startDate = startDate;
    }

    public Integer getExhibitId() {
        return exhibitId;
    }

    public int getDurationSeconds() {
        return durationSeconds;
    }

    public List<Integer> getActions() {
        return actions;
    }

    public Timestamp getStartDate() {
        return startDate;
    }

    @Override
    public String toString() {
        return "RaportEvent{" +
               "exhibitId=" + exhibitId +
               ", durationSeconds=" + durationSeconds +
               ", startDate=" + startDate +
               ", actions=" + actions +
               '}';
    }
}
