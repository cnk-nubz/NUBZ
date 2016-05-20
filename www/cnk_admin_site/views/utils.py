import os
os.environ['DJANGO_SETTINGS_MODULE'] = 'cnk.settings'
from django.conf import settings
from django.template.loader import render_to_string
from django.http import HttpResponse, HttpResponseRedirect, JsonResponse, QueryDict, HttpResponseForbidden, HttpResponseServerError
from ..thrift_communication import ThriftCommunicator, fromThrift
from enums import QuestionType
thriftCommunicator = ThriftCommunicator.ThriftCommunicator()

def get_const(name):
    return getattr(settings, name, None)

def set_const(name, value):
    setattr(settings, name, value)

def maybe(f, t):
    if t is None:
        return None
    else:
        return f(t)

def compose(f, g): return lambda x: f(g(x))

def handleException(ex):
    exceptionType = type(ex).__name__
    if exceptionType == "InternalError":
        return HttpResponseServerError(get_const("INTERNAL_ERROR"))
    elif exceptionType == "InvalidData":
        return HttpResponseForbidden(get_const("INVALID_DATA_ERROR"))
    elif exceptionType == "DuplicateName":
        return HttpResponseForbidden(get_const("DUPLICATE_NAME_ERROR"))
    elif exceptionType == "ElementInUse":
        return HttpResponseForbidden(get_const("ELEMENT_IN_USE_ERROR"))
    else:
        return HttpResponseServerError(str(ex))


def getMapImageInfo():
    floorTiles = thriftCommunicator.getMapImages()
    if not floorTiles.keys():
        return {}

    floorTilesInfo = {}
    for i in xrange(0, 1 + max(floorTiles.keys())):
        if not (i in floorTiles.keys()):
            floorTilesInfo[i] = {}
            continue
        # just keep the zoomLevels, no need for more
        floorTilesInfo[i] = fromThrift.mapImage(floorTiles[i])['zoomLevels']
    return floorTilesInfo


def mergeQuestions(questionsList):
    questions = list()
    for qType in QuestionType:
        if qType == QuestionType.SIMPLE:
            toAdd = questionsList['simpleQuestions']
        elif qType == QuestionType.MULTIPLE:
            toAdd = questionsList['multipleChoiceQuestions']
        elif qType == QuestionType.SORT:
            toAdd = questionsList['sortQuestions']
        for q in toAdd:
            q['type'] = qType.value
        questions.append(toAdd)
    its = [iter(qList) for qList in questions]
    return [next(its[i]) for i in questionsList['questionsOrder']]


def markQuestionAsNew(questions, newId, newType):
    for q in questions:
        if q['questionId'] == newId and q['type'] == newType:
            q['isNew'] = True
            break

def markActionAsNew(actions, newId):
    for a in actions:
        if a['actionId'] == newId:
            a['isNew'] = True
            break

def render_to_stringNNL(name, d = {}):
    return render_to_string(name, d).replace('\n', '')
