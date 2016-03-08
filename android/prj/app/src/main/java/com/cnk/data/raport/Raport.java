package com.cnk.data.raport;

import com.cnk.data.experiment.answers.SurveyAnswers;

import java.io.Serializable;
import java.util.ArrayList;
import java.util.List;

public class Raport implements Serializable {
    private Integer id;
    private List<RaportEvent> history;
    private SurveyAnswers preSurveyAnswers;
    private SurveyAnswers postSurveyAnswers;

    public Raport(Integer id, SurveyAnswers preSurveyAnswers, SurveyAnswers postSurveyAnswers) {
        this.id = id;
        this.preSurveyAnswers = preSurveyAnswers;
        this.postSurveyAnswers = postSurveyAnswers;
        this.history = new ArrayList<>();
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

    public SurveyAnswers getPreSurveyAnswers() {
        return preSurveyAnswers;
    }

    public SurveyAnswers getPostSurveyAnswers() {
        return postSurveyAnswers;
    }

    @Override
    public String toString() {
        return "Raport{" +
               "postSurveyAnswers=" + postSurveyAnswers +
               ", preSurveyAnswers=" + preSurveyAnswers +
               ", history=" + history +
               '}';
    }
}
