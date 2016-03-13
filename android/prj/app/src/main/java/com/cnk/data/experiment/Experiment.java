package com.cnk.data.experiment;

import android.support.annotation.NonNull;

import java.util.List;

public class Experiment {
    private Integer id;
    private String name;
    private List<Action> exhibitActions;
    private List<Action> breakActions;
    private Survey preSurvey;
    private Survey postSurvey;

    public Experiment(Integer id,
                      String name,
                      List<Action> exhibitActions,
                      List<Action> breakActions,
                      Survey preSurvey,
                      Survey postSurvey) {
        this.id = id;
        this.name = name;
        this.exhibitActions = exhibitActions;
        this.breakActions = breakActions;
        this.preSurvey = preSurvey;
        this.postSurvey = postSurvey;
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

    public Survey getSurvey(@NonNull Survey.SurveyType type) {
        switch (type) {
            case BEFORE:
                return preSurvey;
            case AFTER:
                return postSurvey;
            default:
                throw new RuntimeException();
        }
    }
}
