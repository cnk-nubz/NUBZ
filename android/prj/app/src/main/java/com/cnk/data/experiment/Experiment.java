package com.cnk.data.experiment;

import java.util.List;

public class Experiment {
    private Integer id;
    private String name;
    private List<Action> exhibitActions;
    private List<Action> breakActions;

    public Experiment(Integer id, String name, List<Action> exhibitActions, List<Action> breakActions) {
        this.id = id;
        this.name = name;
        this.exhibitActions = exhibitActions;
        this.breakActions = breakActions;
    }

    public Integer getId() {
        return id;
    }

    public String getName() {
        return name;
    }

    public List<Action> getExhibitActions() {
        return exhibitActions;
    }

    public List<Action> getBreakActions() {
        return breakActions;
    }
}
