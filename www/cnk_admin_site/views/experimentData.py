import json, os
from utils                  import standardAjaxCall, mergeQuestions, markQuestionAsNew, markActionAsNew
from django.http            import JsonResponse, QueryDict
from ..thrift_communication import ThriftCommunicator, toThrift, fromThrift
from enums                  import QuestionType
thriftCommunicator = ThriftCommunicator.ThriftCommunicator()


@standardAjaxCall
def question(request):
    if request.method == "POST":
        return createQuestion(request)
    elif request.method == "DELETE":
        return removeQuestion(request)


def createQuestion(request):
    data = json.loads(request.POST.get("jsonData"))
    questionType = data.get('type')
    if questionType == QuestionType.SIMPLE.value:
        tData = toThrift.createSimpleQuestionRequest(data)
        question = fromThrift.simpleQuestion(
            thriftCommunicator.createSimpleQuestion(tData))
    elif questionType == QuestionType.SORT.value:
        tData = toThrift.createSortQuestionRequest(data)
        question = fromThrift.sortQuestion(
            thriftCommunicator.createSortQuestion(tData))
    elif questionType == QuestionType.MULTIPLE.value:
        tData = toThrift.createMultipleChoiceQuestionRequest(data)
        question = fromThrift.multipleChoiceQuestion(
            thriftCommunicator.createMultipleChoiceQuestion(tData))

    responseData = mergeQuestions(
        fromThrift.questionsList(thriftCommunicator.getAllQuestions()))
    markQuestionAsNew(responseData, question['questionId'], questionType)
    return JsonResponse({
        'questionsList': responseData
    })


def removeQuestion(request):
    DELETE = QueryDict(request.body)
    data = json.loads(DELETE.get("jsonData"))
    questionType = data['type']
    questionId = data['questionId']
    if questionType == QuestionType.SIMPLE.value:
        thriftCommunicator.removeSimpleQuestion(questionId)
    elif questionType == QuestionType.SORT.value:
        thriftCommunicator.removeSortQuestion(questionId)
    elif questionType == QuestionType.MULTIPLE.value:
        thriftCommunicator.removeMultipleChoiceQuestion(questionId)
    return JsonResponse({
        'questionsList': mergeQuestions(
            fromThrift.questionsList(thriftCommunicator.getAllQuestions())
        )
    })


@standardAjaxCall
def action(request):
    if request.method == "POST":
        return createAction(request)
    else:
        return removeAction(request)


def createAction(request):
    data = json.loads(request.POST.get("jsonData"))
    tData = toThrift.createActionRequest(data)
    newAction = fromThrift.action(
        thriftCommunicator.createAction(tData))

    responseData = map(fromThrift.action, thriftCommunicator.getAllActions())
    markActionAsNew(responseData, newAction['actionId'])
    return JsonResponse({
        'actionsList': responseData
    })


def removeAction(request):
    DELETE = QueryDict(request.body)
    actionId = json.loads(DELETE.get('jsonData'))['actionId']
    thriftCommunicator.removeAction(actionId)
    return JsonResponse({
        'actionsList':
            map(fromThrift.action, thriftCommunicator.getAllActions())
    })
