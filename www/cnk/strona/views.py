# --*-- encoding: utf-8 --*--
import sys
import os
import json
import logging
import StringIO
from enum import Enum
from django.shortcuts import render
from django.views.decorators.csrf import ensure_csrf_cookie
from django.http import HttpResponse, HttpResponseRedirect, JsonResponse
from django.template import RequestContext, loader
from django.template.loader import render_to_string
from django.core.urlresolvers import reverse
from django.views.decorators.csrf import ensure_csrf_cookie
from django.utils.safestring import mark_safe
from django.conf import settings
os.environ['DJANGO_SETTINGS_MODULE'] = 'cnk.settings'
from .models import MapUploader
from .forms import MapUploadForm
from ThriftCommunicator import ThriftCommunicator

thriftCommunicator = ThriftCommunicator()
class InternalError(Exception):
    pass
class InvalidData(Exception):
    pass
class DuplicateName(Exception):
    pass

def get_const(name):
    return getattr(settings, name, None)


class uploadError(Enum):
    SUCCESS = 1
    NOT_AN_IMAGE = 2
    SERVER_PROBLEM = 3
    WRONG_REQUEST_METHOD = 4
    INVALID_DATA = 5


class QuestionType(Enum):
    SIMPLE = 0
    MULTIPLE = 1
    SORT = 2


class ActiveLink(Enum):
    JUST_MAP = '0'
    EDIT_MAP = '1'
    QUESTIONS_ACTIONS_PAGE = '2'
    EXPERIMENTS_PAGE = '3'


def _pingServer():
    result = thriftCommunicator.ping(1, 'x')
    return result == 1


def _getMapImageInfo():
    floorTiles = thriftCommunicator.getMapImageTiles()
    floorTilesInfo = {}
    for i in xrange(0, 2):
        if not (i in floorTiles.keys()):
            floorTilesInfo[i] = {}
            continue
        floorTilesInfo[i] = {
            idx: {
                'tileWidth': zoom.tileSize.width,
                'tileHeight': zoom.tileSize.height,
                'scaledWidth': zoom.scaledSize.width,
                'scaledHeight': zoom.scaledSize.height
            } for idx, zoom in enumerate(floorTiles[i].zoomLevels)
        }
    return floorTilesInfo


def _getExhibits():
    exhibits = thriftCommunicator.getAllExhibits()
    exhibitsList = list()
    for e in exhibits:
        frame = None
        if e.mapFrame is not None:
            frame = {
                'x': e.mapFrame.frame.x,
                'y': e.mapFrame.frame.y,
                'width': e.mapFrame.frame.size.width,
                'height': e.mapFrame.frame.size.height,
                'mapLevel': e.mapFrame.floor
            }
        exhibitsList.append({
            'name': e.name,
            'id': e.exhibitId,
            'frame': frame,
            'colorHex': _getHtmlColorHex(e.rgbHex)
        })
    return exhibitsList


def _getHtmlColorHex(intVal):
    return '#' + hex(intVal).split('x')[1].rjust(6, '0')


@ensure_csrf_cookie
def getMapPage(request, file, activeLink):
    try:
        _pingServer()
        floorTilesInfo = _getMapImageInfo()
        exhibits = _getExhibits()
    except Exception as ex:
        return HttpResponse('<h1>{}</h1>'.format(str(ex)))

    template = loader.get_template(file)
    urlFloor0 = getattr(settings, 'FLOOR0_TILES_DIRECTORY', '')
    urlFloor1 = getattr(settings, 'FLOOR1_TILES_DIRECTORY', '')

    context = RequestContext(request, {
        'activeFloor': 0,
        'exhibits': exhibits,
        'floorTilesInfo': floorTilesInfo,
        'urlFloor0': urlFloor0,
        'urlFloor1': urlFloor1,
        'activeLink': activeLink
    })
    return HttpResponse(template.render(context))


def androidApp(request):
    return HttpResponseRedirect("/static/android_app/example")


@ensure_csrf_cookie
def index(request):
    if request.META.get("HTTP_USER_AGENT", '').lower().find("android") > 0:
        return HttpResponseRedirect("/static/android_app/example")
    return getMapPage(request, 'map/justMap.html', ActiveLink.JUST_MAP.value)


@ensure_csrf_cookie
def editMapPage(request):
    return getMapPage(request, 'map/editMap.html', ActiveLink.EDIT_MAP.value)


def uploadImage(request):
    if request.method != 'POST':
        data = {
            "err": uploadError.WRONG_REQUEST_METHOD.value,
        }
        return JsonResponse(data)

    form = MapUploadForm(request.POST, request.FILES)
    if not form.is_valid():
        data = {
            "err": uploadError.NOT_AN_IMAGE.value,
        }
        return JsonResponse(data)

    m = MapUploader(image=form.cleaned_data['image'])
    m.save()
    floor = form.cleaned_data['floor']
    # send information about update
    filename = m.image.name
    # extract filename
    try:
        setResult = thriftCommunicator.setMapImage(
            floor, os.path.basename(filename))
        floorTilesInfo = _getMapImageInfo()
    except Exception as ex:
        data = {
            "err": uploadError.SERVER_PROBLEM.value,
        }
        return JsonResponse(data)

    if floor == 0:
        floorUrl = getattr(settings, 'FLOOR0_TILES_DIRECTORY', '')
    else:
        floorUrl = getattr(settings, 'FLOOR1_TILES_DIRECTORY', '')

    data = {
        "err": uploadError.SUCCESS.value,
        "floor": floor,
        "floorTilesInfo": floorTilesInfo,
        "floorUrl": floorUrl,
        "exhibits": _getExhibits()
    }
    return JsonResponse(data)


def updateExhibitPosition(request):
    data = {
        "success": False
    }
    if request.method != 'POST':
        return JsonResponse(data)
    jsonData = request.POST.get("jsonData")
    frame = json.loads(jsonData)

    try:
        thriftCommunicator.setExhibitFrame(frame)
    except Exception as ex:
        data['message'] = str(ex)
        return JsonResponse(data)

    data = {
        "success": True,
        "id": int(frame['id']),
        "x": frame['x'],
        "y": frame['y'],
        "width": frame['width'],
        "height": frame['height']
    }
    return JsonResponse(data)


def createNewExhibit(request):
    return _exhibitRequestsUnified(
        request, thriftCommunicator.createNewExhibit)


def updateExhibit(request):
    return _exhibitRequestsUnified(request, thriftCommunicator.updateExhibit)


def _exhibitRequestsUnified(request, funToCall):
    data = {
        "success": False
    }
    if request.method != 'POST':
        return JsonResponse(data)
    jsonData = request.POST.get("jsonData")
    exhibitRequest = json.loads(jsonData)

    try:
        newExhibit = funToCall(exhibitRequest)
    except Exception as ex:
        if type(ex).__name__ == "DuplicateName":
            message = get_const("DEFAULT_CONSTANTS")['utils']['text']['nameDuplicatedError']
        else:
            message = "Wystąpił nieoczekiwany błąd. Spróbuj ponownie za chwilę. ({})".format(
            str(ex))
        data['message'] = message
        data['exceptionType'] = type(ex).__name__
        return JsonResponse(data)

    if newExhibit.mapFrame:
        frame = newExhibit.mapFrame
        exhibitFrame = {
            "x": frame.frame.x,
            "y": frame.frame.y,
            "width": frame.frame.size.width,
            "height": frame.frame.size.height,
            "mapLevel": frame.floor
        }
    else:
        exhibitFrame = None
    data = {
        "success": True,
        "id": int(newExhibit.exhibitId),
        "name": newExhibit.name,
        "rgbHex": '#' + hex(newExhibit.rgbHex).split('x')[1].upper().rjust(6, '0'),
        "frame": exhibitFrame
    }
    return JsonResponse(data)


def getDialog(dialogName):
    contextDict = {
        'data': get_const(dialogName)['data']
    }
    html = render_to_string('dialog/dialog.html', contextDict)
    retDict = {
        'data': get_const(dialogName),
        'html': html.replace("\n", "")
    }
    return JsonResponse(retDict)


def getColorPickerPopoverContent(request):
    colorsList = get_const("POPOVER_COLORS")
    popoverButtonHtml = render_to_string(
        'dialog/popoverColorsPicker.html', {'colorsList': colorsList})
    return JsonResponse({'html': popoverButtonHtml})


def getExhibitDialog(request):
    return getDialog("EXHIBIT_DIALOG")


def getSimpleQuestionDialog(request):
    return getDialog("SIMPLE_QUESTION_DIALOG")


def getMultipleChoiceQuestionDialog(request):
    return getDialog("MULTIPLE_CHOICE_QUESTION_DIALOG")


def getExhibitPanel(request):
    html = render_to_string('exhibitPanel/exhibitPanel.html')
    return JsonResponse({'html': html})


def getExhibitListElement(request):
    html = render_to_string('exhibitPanel/exhibitListElement.html')
    return JsonResponse({'html': html})


def getSortQuestionDialog(request):
    return getDialog("SORT_QUESTION_DIALOG")


def getActionDialog(request):
    return getDialog("NEW_ACTION_DIALOG")


def getChangeMapDialog(request):
    floor = request.GET.get("floor")
    html = render_to_string('dialog/changeMap.html', {'floor': floor})
    return JsonResponse({'html': html.replace("\n", "")})


def getChooseQuestionTypeDialog(request):
    html = render_to_string('dialog/chooseQuestionType.html')
    return JsonResponse({'html': html.replace("\n", "")})

def getChangeNameDialog(request):
    return getDialog("CHANGE_EXPERIMENT_NAME_DIALOG")

HTMLRequests = {
    'simpleQuestionDialog': getSimpleQuestionDialog,
    'sortQuestionDialog': getSortQuestionDialog,
    'multipleChoiceQuestionDialog': getMultipleChoiceQuestionDialog,
    'actionDialog': getActionDialog,
    'chooseQuestionTypeDialog': getChooseQuestionTypeDialog,
    'changeMapDialog': getChangeMapDialog,
    'exhibitDialog': getExhibitDialog,
    'colorPickerPopover': getColorPickerPopoverContent,
    'exhibitPanel': getExhibitPanel,
    'exhibitListElement': getExhibitListElement,
    'changeNameDialog': getChangeNameDialog
}

def getHTML(request):
    name = request.GET.get("name")
    return HTMLRequests[name](request)

def _parseQuestions(allQuestions, newId=None, newType=None):
    idxSimple = 0
    idxMultiple = 0
    idxSort = 0
    questionsList = []
    # sort questions in order
    for order in allQuestions.questionsOrder:
        if order == QuestionType.SIMPLE.value:
            q = allQuestions.simpleQuestions[idxSimple]
            idxSimple += 1
            questionsList.append({
                'questionId': q.questionId,
                'isNew': newId == q.questionId and order == newType,
                'type': QuestionType.SIMPLE.value,
                'name': q.name,
                'question': q.question,
                'answerType': q.answerType
            })
        elif order == QuestionType.MULTIPLE.value:
            q = allQuestions.multipleChoiceQuestions[idxMultiple]
            idxMultiple += 1
            questionsList.append({
                'questionId': q.questionId,
                'isNew': newId == q.questionId and order == newType,
                'type': QuestionType.MULTIPLE.value,
                'name': q.name,
                'question': q.question,
                'singleAnswer': q.singleAnswer,
                'options': [o.text for o in q.options]
            })
        else:
            q = allQuestions.sortQuestions[idxSort]
            idxSort += 1
            questionsList.append({
                'questionId': q.questionId,
                'isNew': newId == q.questionId and order == newType,
                'type': QuestionType.SORT.value,
                'name': q.name,
                'question': q.question,
                'options': [o.text for o in q.options]
            })
    return questionsList


def _parseActions(allActions, newId=None):
    actionsList = [{
        'actionId': a.actionId,
        'isNew': newId == a.actionId,
        'text': a.text
    } for a in allActions]
    return actionsList


def _getExperiment(experimentId):
    experiment = thriftCommunicator.getExperiment(int(experimentId))
    return {
        'experimentId': experiment.experimentId,
        'name': experiment.name,
        'questionsBefore': _parseQuestions(experiment.surveyBefore),
        'questionsAfter': _parseQuestions(experiment.surveyAfter),
        'exhibitActions': _parseActions(experiment.exhibitActions),
        'breakActions': _parseActions(experiment.breakActions)
    }

def _getRaports(experimentId):
    raports = thriftCommunicator.getAllRaports(int(experimentId))
    return raports
    raportsList = list()
    for r in raports:
        raportsList.append({
            'raportId': r.raportId,
            'name': r.name,
            'date': {
                'day': r.date.day,
                'month': r.date.month,
                'year': r.date.year
            }
        })
    return raportsList

def _newExperimentReadonlyPage(request):
    experimentId = request.GET.get("id")
    try:
        result = {
            'success': True,
            'readonlyExperimentActionRow': render_to_string('list/row/readonlyExperimentActionRow.html'),
            'readonlyExperimentQuestionRow': render_to_string('list/row/readonlyExperimentQuestionRow.html'),
            'raportRow': render_to_string('list/row/raportRow.html'),
            'experimentData': _getExperiment(experimentId),
            'raportsList': _getRaports(experimentId),
            'tableList': render_to_string('list/dataList.html')}
    except Exception as ex:
        result = {
            'success': False,
            'message': str(ex)
        }
    template = loader.get_template('readonlyExperiment.html')
    return HttpResponse(template.render(RequestContext(request, result)))


def newExperimentPage(request):
    readonly = request.GET.get("readonly")
    experimentId = request.GET.get("id")
    try:
        if not (readonly is None):
            return _newExperimentReadonlyPage(request)
        result = {
            'success': True,
            'experimentData': _getExperiment(experimentId) if experimentId else None,
            'questionsList': _parseQuestions(
                thriftCommunicator.getAllQuestions()),
            'actionsList': _parseActions(
                thriftCommunicator.getAllActions()),
            'chooseQuestionRow': render_to_string('list/row/chooseQuestionRow.html'),
            'chooseActionRow': render_to_string('list/row/chooseActionRow.html'),
            'experimentActionRow': render_to_string('list/row/experimentActionRow.html'),
            'experimentQuestionRow': render_to_string('list/row/experimentQuestionRow.html'),
            'tableList': render_to_string('list/dataList.html')}
    except Exception as ex:
        result = {
            'success': False,
            'message': str(ex)
        }
    template = loader.get_template('newExperimentPage.html')
    return HttpResponse(template.render(RequestContext(request, result)))


def questionsAndActionsPage(request):
    try:
        result = {
            'success': True,
            'activeLink': ActiveLink.QUESTIONS_ACTIONS_PAGE.value,
            'questionsList': _parseQuestions(
                thriftCommunicator.getAllQuestions()),
            'actionsList': _parseActions(
                thriftCommunicator.getAllActions()),
            'showQuestionRow': render_to_string('list/row/showQuestionRow.html'),
            'showActionRow': render_to_string('list/row/showActionRow.html'),
            'tableList': render_to_string('list/dataList.html')}
    except Exception as ex:
        result = {
            'success': False,
            'exceptionType': type(ex).__name__,
            'activeLink': ActiveLink.QUESTIONS_ACTIONS_PAGE.value,
            'message': str(ex)
        }
    template = loader.get_template('questionsAndActionsPage.html')
    return HttpResponse(template.render(RequestContext(request, result)))


def createSimpleQuestion(request):
    data = json.loads(request.POST.get("jsonData"))
    try:
        simpleQuestion = thriftCommunicator.createSimpleQuestion(data)
        result = {
            'success': True,
            'questionsList': _parseQuestions(
                thriftCommunicator.getAllQuestions(),
                simpleQuestion.questionId,
                QuestionType.SIMPLE.value
            )
        }
    except Exception as ex:
        result = {
            'success': False,
            'exceptionType': type(ex).__name__,
            'message': str(ex)
        }
    return JsonResponse(result)


def createMultipleChoiceQuestion(request):
    data = json.loads(request.POST.get("jsonData"))
    try:
        multipleChoiceQuestion = thriftCommunicator.createMultipleChoiceQuestion(data)
        result = {
            'success': True,
            'questionsList': _parseQuestions(
                thriftCommunicator.getAllQuestions(),
                multipleChoiceQuestion.questionId,
                QuestionType.MULTIPLE.value
            )
        }
    except Exception as ex:
        result = {
            'success': False,
            'exceptionType': type(ex).__name__,
            'message': str(ex)
        }
    return JsonResponse(result)


def createSortQuestion(request):
    data = json.loads(request.POST.get("jsonData"))
    try:
        sortQuestion = thriftCommunicator.createSortQuestion(data)
        result = {
            'success': True,
            'questionsList': _parseQuestions(
                thriftCommunicator.getAllQuestions(),
                sortQuestion.questionId,
                QuestionType.SORT.value
            )
        }
    except Exception as ex:
        result = {
            'success': False,
            'exceptionType': type(ex).__name__,
            'message': str(ex)
        }
    return JsonResponse(result)


def createAction(request):
    data = json.loads(request.POST.get("jsonData"))
    try:
        action = thriftCommunicator.createAction(data)
        result = {
            'success': True,
            'actionsList': _parseActions(
                thriftCommunicator.getAllActions(),
                action.actionId
            )
        }

    except Exception as ex:
        result = {
            'success': False,
            'exceptionType': type(ex).__name__,
            'message': str(ex)
        }
    return JsonResponse(result)


def saveExperiment(request):
    data = json.loads(request.POST.get("jsonData"))
    try:
        if "experimentId" in data.keys():
            thriftCommunicator.updateExperiment(data)
        else:
            thriftCommunicator.createExperiment(data)
        result = {
            'success': True,
        }
    except Exception as ex:
        if type(ex).__name__ == "DuplicateName":
            message = get_const("DEFAULT_CONSTANTS")['utils']['text']['nameDuplicatedError']
        else:
            message = "Wystąpił nieoczekiwany błąd. Spróbuj ponownie za chwilę. ({})".format(
            str(ex))
        result = {
            'success': False,
            'exceptionType': type(ex).__name__,
            'message': message
        }
    return JsonResponse(result)


def _parseExperiments(experiments):
    experimentsList = list()
    for e in experiments:
        experimentsList.append({
            'experimentId': e.id,
            'name': e.name,
            'startDate': {
                'day': e.startDate.day,
                'month': e.startDate.month,
                'year': e.startDate.year
            } if (not e.startDate is None) else None,
            'finishDate': {
                'day': e.finishDate.day,
                'month': e.finishDate.month,
                'year': e.finishDate.year
            } if not (e.finishDate is None) else None
        })
    return experimentsList


def _getFinishedExperiments():
    return _parseExperiments(thriftCommunicator.getFinishedExperiments())


def _getReadyExperiments():
    return _parseExperiments(thriftCommunicator.getReadyExperiments())


def _getActiveExperiment():
    activeExperiment = thriftCommunicator.getActiveExperiment()
    if activeExperiment.info is None:
        return None
    return _parseExperiments([activeExperiment.info])[0]


@ensure_csrf_cookie
def experimentsPage(request):
    template = loader.get_template('experimentsPage.html')
    try:
        context = {
            'success': True,
            'activeLink': ActiveLink.EXPERIMENTS_PAGE.value,
            'finishedExperiments': _getFinishedExperiments(),
            'readyExperiments': _getReadyExperiments(),
            'activeExperiment': _getActiveExperiment(),
            'finishedExperimentRow': render_to_string('list/row/finishedExperimentRow.html'),
            'readyExperimentRow': render_to_string('list/row/readyExperimentRow.html'),
            'confirmationMessages': get_const("EXPERIMENT_CONFIRMATION_MESSAGES"),
            'tableList': render_to_string('list/dataList.html')}
    except Exception as ex:
        context = {
            'success': False,
            'message': str(ex)
        }
    return HttpResponse(template.render(context))


def startExperiment(request):
    experimentId = int(request.POST.get("id"))
    try:
        thriftCommunicator.startExperiment(experimentId)
        result = {
            'success': True
        }
    except Exception as ex:
        result = {
            'success': False,
            'message': str(ex)
        }
    return JsonResponse(result)


def finishExperiment(request):
    try:
        thriftCommunicator.finishExperiment()
        result = {
            'success': True
        }
    except Exception as ex:
        result = {
            'success': False,
            'message': str(ex)
        }
    return JsonResponse(result)

def cloneExperiment(request):
    data = json.loads(request.POST.get("jsonData"))
    try:
        thriftCommunicator.cloneExperiment(data['experimentId'], data['newName'])
        result = {
            'success': True
        }
    except Exception as ex:
        result = {
            'success': False,
            'exceptionType': "{}".format(type(ex).__name__),
            'message': str(ex)
        }
    return JsonResponse(result)

def getAllExhibits(request):
    try:
        exhibits = _getExhibits()
        result = {
            'success': True,
            'exhibits': exhibits
        }
    except Exception as ex:
        result = {
            'success': False,
            'message': 'Nie udało sie pobrać listy eksponatów. ({})'.format(
                str(ex))}
    return JsonResponse(result)

def reportError(request):
    data = json.loads(request.POST.get("jsonData"))
    logger = logging.getLogger('jsLogger')
    logger.error(": {0} in {1}, {2}".format(data['url'], data['lineNumber'], data['errorMsg']))
    return JsonResponse()

def getRaport(request):
    raportId = request.GET.get("id")
    myfile = StringIO.StringIO()
    #TODO: write from file
    myfile.write("Raporcik dla id: {}".format(raportId))
    response = HttpResponse(content_type='text/plain')
    response['Content-Disposition'] = "attachment; filename=raport-{}.txt".format(raportId)
    response['Content-Length'] = myfile.tell()
    response.write(myfile.getvalue())
    return response

def getAllRaports(request):
    myfile = StringIO.StringIO()
    #TODO: write from file
    myfile.write("Wszystkie raporty zzipowane")
    response = HttpResponse(content_type='text/plain')
    response['Content-Disposition'] = "attachment; filename=rapory-wszystkie.txt"
    response['Content-Length'] = myfile.tell()
    response.write(myfile.getvalue())
    return response
