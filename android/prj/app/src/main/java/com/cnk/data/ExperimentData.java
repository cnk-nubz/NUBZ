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

    public void setExhibitActions(List<Action> exhibitActions) {
        this.exhibitActions = exhibitActions;
    }

    public List<Action> getBreakActions() {
        return breakActions;
    }

    public void setBreakActions(List<Action> breakActions) {
        this.breakActions = breakActions;
    }
}
