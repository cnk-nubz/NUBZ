package com.cnk.data;

import java.util.List;

public class ExperimentData {
    List<Action> exhibitActions;
    List<Action> breakActions;

    public ExperimentData(List<Action> exhibitActions, List<Action> breakActions) {
        this.exhibitActions = exhibitActions;
        this.breakActions = breakActions;
    }

    public List<Action> getExhibitActions() {
        return exhibitActions;
    }

    public List<Action> getBreakActions() {
        return breakActions;
    }
}
