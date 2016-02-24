package com.cnk.data.experiment;

import java.util.Queue;

public class Survey {
    public enum QuestionType {
        SIMPLE,
        MULTIPLE_CHOICE;
    }

    private Queue<QuestionType> questionOrder;
    private Queue<SimpleQuestion> simpleQuestions;

    public Survey(Queue<QuestionType> questionOrder, Queue<SimpleQuestion> simpleQuestions) {
        this.questionOrder = questionOrder;
        this.simpleQuestions = simpleQuestions;
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
}
