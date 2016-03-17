package com.cnk.data.experiment.survey;

import com.cnk.data.experiment.survey.answers.SurveyAnswers;
import com.cnk.data.experiment.survey.questions.MultipleChoiceQuestion;
import com.cnk.data.experiment.survey.questions.SimpleQuestion;
import com.cnk.data.experiment.survey.questions.SortQuestion;

import java.util.Queue;

public class Survey {
    private Queue<QuestionType> questionOrder;
    private Queue<SimpleQuestion> simpleQuestions;
    private Queue<MultipleChoiceQuestion> multipleChoiceQuestions;
    private Queue<SortQuestion> sortQuestions;
    private SurveyAnswers surveyAnswers;

    public Survey(Queue<QuestionType> questionOrder,
                  Queue<SimpleQuestion> simpleQuestions,
                  Queue<MultipleChoiceQuestion> multipleChoiceQuestions,
                  Queue<SortQuestion> sortQuestions) {
        this.questionOrder = questionOrder;
        this.simpleQuestions = simpleQuestions;
        this.multipleChoiceQuestions = multipleChoiceQuestions;
        this.sortQuestions = sortQuestions;
        this.surveyAnswers = new SurveyAnswers();
    }

    public Integer getRemainingQuestionsCount() {
        return questionOrder.size();
    }

    public QuestionType popNextQuestionType() {
        return questionOrder.remove();
    }

    public SimpleQuestion popNextSimpleQuestion() {
        return simpleQuestions.remove();
    }

    public MultipleChoiceQuestion popNextMultipleChoiceQuestion() {
        return multipleChoiceQuestions.remove();
    }

    public SortQuestion popNextSortQuestions() {
        return sortQuestions.remove();
    }

    public SurveyAnswers getSurveyAnswers() {
        return surveyAnswers;
    }

    public enum QuestionType {
        SIMPLE,
        MULTIPLE_CHOICE,
        SORT;
    }

    public enum SurveyType {
        BEFORE,
        AFTER;
    }
}
