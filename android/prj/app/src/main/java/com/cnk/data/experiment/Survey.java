package com.cnk.data.experiment;

import com.cnk.data.experiment.questions.MultipleChoiceQuestion;
import com.cnk.data.experiment.questions.SimpleQuestion;
import com.cnk.data.experiment.questions.SortQuestion;

import java.util.Queue;

public class Survey {
    private Queue<QuestionType> questionOrder;
    private Queue<SimpleQuestion> simpleQuestions;
    private Queue<MultipleChoiceQuestion> multipleChoiceQuestions;
    private Queue<SortQuestion> sortQuestions;

    public Survey(Queue<QuestionType> questionOrder,
                  Queue<SimpleQuestion> simpleQuestions,
                  Queue<MultipleChoiceQuestion> multipleChoiceQuestions,
                  Queue<SortQuestion> sortQuestions
    ) {
        this.questionOrder = questionOrder;
        this.simpleQuestions = simpleQuestions;
        this.multipleChoiceQuestions = multipleChoiceQuestions;
        this.sortQuestions = sortQuestions;
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
