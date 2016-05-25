package com.cnk.data.raports;

import com.cnk.data.experiment.survey.answers.SurveyAnswers;

import java.io.Serializable;
import java.util.ArrayList;
import java.util.List;

public class Raport implements Serializable {
    public enum State {
        IN_PROGRESS,
        READY_TO_SEND,
        SENT
    }

    private Integer id;
    private Integer experimentId;
    private List<RaportEvent> history;
    private SurveyAnswers preSurveyAnswers;
    private SurveyAnswers postSurveyAnswers;
    private State state;
    private Timestamp endDate;
    private Timestamp startDate;

    public Raport() {
    }

    public Raport(Integer id,
                  Timestamp startDate,
                  Integer experimentId,
                  SurveyAnswers preSurveyAnswers,
                  SurveyAnswers postSurveyAnswers) {
        this.id = id;
        this.startDate = startDate;
        this.experimentId = experimentId;
        this.preSurveyAnswers = preSurveyAnswers;
        this.postSurveyAnswers = postSurveyAnswers;
        this.history = new ArrayList<>();
        this.state = State.IN_PROGRESS;
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

    public Integer getExperimentId() {
        return experimentId;
    }

    public SurveyAnswers getPreSurveyAnswers() {
        return preSurveyAnswers;
    }

    public SurveyAnswers getPostSurveyAnswers() {
        return postSurveyAnswers;
    }

    public State getState() {
        return state;
    }

    public void markAsReady() {
        state = State.READY_TO_SEND;
    }

    public void markAsSent() {
        state = State.SENT;
    }

    public void setEndDate(Timestamp d) {
        endDate = d;
    }

    public Timestamp getEndDate() {
        return endDate;
    }

    public Timestamp getStartDate() {
        return startDate;
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
