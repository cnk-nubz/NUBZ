from structs.ttypes import *
from ..views.utils  import maybe


#################################################
## Ping
#################################################


def helloMsg(d):
    return HelloMsg(d['num'], d['msg'])


#################################################
## Map
#################################################


def size(d):
    return Size(d['width'], d['height'])


def frame(d):
    return Frame(d['x'], d['y'], size(d['size']))


def mapFrame(d):
    return MapFrame(frame(d['frame']), d['floor'])


def setMapImageRequest(d):
    return SetMapImageRequest(d['floor'], d['filename'])


#################################################
## Exhibits
#################################################


def setExhibitFrameRequest(d):
    return SetExhibitFrameRequest(d['exhibitId'], frame(d['frame']))


def createExhibitRequest(d):
    return CreateExhibitRequest(
        d['name'].encode('utf-8'),
        d['rgbHex'],
        d.get('floor'),
        maybe(mapFrame, d.get('visibleFrame')))


def updateExhibitRequest(d):
    return UpdateExhibitRequest(
        d['exhibitId'],
        d['rgbHex'],
        d.get('floor'),
        maybe(mapFrame, d.get('visibleFrame')))


#################################################
## Actions
#################################################


def createActionRequest(d):
    return CreateActionRequest(d['text'].encode('utf-8'))


#################################################
## Simple Question
#################################################


def createSimpleQuestionRequest(d):
    if d['answerType'].upper() == "TEXT":
        tAnswerType = SimpleQuestionAnswerType.TEXT
    else:
        tAnswerType = SimpleQuestionAnswerType.NUMBER
    return CreateSimpleQuestionRequest(
        d['name'].encode('utf-8'),
        d['question'].encode('utf-8'),
        tAnswerType)


#################################################
## Multiple Choice Question
#################################################


def createMultipleChoiceQuestionRequest(d):
    encodedOptions = [o.encode('utf-8') for o in d['options']]
    return CreateMultipleChoiceQuestionRequest(
        d['name'].encode('utf-8'),
        d['question'],
        d['singleAnswer'],
        encodedOptions)


#################################################
## Sort Question
#################################################


def createSortQuestionRequest(d):
    encodedOptions = [o.encode('utf-8') for o in d['options']]
    return CreateSortQuestionRequest(
        d['name'].encode('utf-8'),
        d['question'], encodedOptions)


#################################################
## Questions List
#################################################


def questionsIdsList(d):
    return QuestionsIdsList(
        d['questionsOrder'],
        d['simpleQuestions'],
        d['multipleChoiceQuestions'],
        d['sortQuestions'])


#################################################
## Experiment
#################################################


def createExperimentRequest(d):
    tSurveyBefore = questionsIdsList(d['surveyBefore'])
    tSurveyAfter  = questionsIdsList(d['surveyAfter'])
    return CreateExperimentRequest(
        d['name'].encode('utf-8'),
        tSurveyBefore,
        d['exhibitActions'],
        d['breakActions'],
        tSurveyAfter)


def cloneRequest(d):
    return CloneRequest(d['experimentId'], d['name'].encode('utf-8'))
