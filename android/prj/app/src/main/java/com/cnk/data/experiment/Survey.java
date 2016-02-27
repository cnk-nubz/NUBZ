package com.cnk.data.experiment;

import com.cnk.data.experiment.questions.MultipleChoiceQuestion;
import com.cnk.data.experiment.questions.SimpleQuestion;

import java.util.Queue;

public class Survey {
    public enum QuestionType {
        SIMPLE,
        MULTIPLE_CHOICE;
    }

    public enum SurveyType {
        BEFORE,
        AFTER;
    }

    private Queue<QuestionType> questionOrder;
    private Queue<SimpleQuestion> simpleQuestions;
    private Queue<MultipleChoiceQuestion> multipleChoiceQuestions;

    public Survey(Queue<QuestionType> questionOrder,
                  Queue<SimpleQuestion> simpleQuestions,
                  Queue<MultipleChoiceQuestion> multipleChoiceQuestions
                 ) {
        this.questionOrder = questionOrder;
        this.simpleQuestions = simpleQuestions;
        this.multipleChoiceQuestions = multipleChoiceQuestions;
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
}
