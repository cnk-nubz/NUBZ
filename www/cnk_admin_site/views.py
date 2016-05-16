# --*-- encoding: utf-8 --*--
import sys
import os
import json
import logging
import copy
import StringIO
from enum import Enum
from django.shortcuts import render
from django.views.decorators.csrf import ensure_csrf_cookie
from django.http import HttpResponse, HttpResponseRedirect, JsonResponse, QueryDict, HttpResponseForbidden, HttpResponseServerError
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

def set_const(name, value):
    setattr(settings, name, value)

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
    thriftCommunicator.ping(1, 'x')


def _getMapImageInfo():
    floorTiles = thriftCommunicator.getMapImages()
    if not floorTiles.keys():
        return {}

    floorTilesInfo = {}
    for i in xrange(0, 1 + max(floorTiles.keys())):
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

    if not floorTilesInfo:
        availableFloors = [0]
    else:
        availableFloors = range(0, 1 + max(floorTilesInfo.keys()))
    set_const("AVAILABLE_FLOORS", availableFloors)
    template = loader.get_template(file)
    urlFloor = getattr(settings, 'FLOOR_TILES_DIRECTORY', '')

    context = RequestContext(request, {
        'activeFloor': 0,
        'exhibits': exhibits,
        'floorTilesInfo': floorTilesInfo,
        'urlFloor': urlFloor,
        'activeLink': activeLink,
        'availableFloors': availableFloors
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
    form = MapUploadForm(request.POST, request.FILES)
    if not form.is_valid():
        data = {
            'success': False,
            'dialog': get_const("NEW_FLOOR_IMAGE_WRONG_FORMAT")
        }
        return JsonResponse(data)

    try:
        m = MapUploader(image=form.cleaned_data['image'])
        m.save()
        floor = form.cleaned_data['floor']
        filename = m.image.name
        setResult = thriftCommunicator.setMapImage(
            floor, os.path.basename(filename))
        floorTilesInfo = _getMapImageInfo()
    except Exception as ex:
        data = {
            'success': False,
            'dialog': {
                'message': get_const("INTERNAL_ERROR")
            }
        }
        return JsonResponse(data)


    dialog = copy.copy(get_const("NEW_FLOOR_IMAGE_SUCCESS"))
    dialog['message'] = "{} {}".format(dialog['message'], floor)
    data = {
        'success': True,
        'dialog': dialog,
        "floor": floor,
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


def exhibit(request):
    try:
        if request.method == "POST":
            return createNewExhibit(request)
        elif request.method == "PUT":
            return updateExhibit(request)
        elif request.method == "DELETE":
            return removeExhibit(request)
        elif request.method == "GET":
            return getAllExhibits(request)
    except Exception as ex:
        exceptionType = type(ex).__name__
        if exceptionType == "DuplicateName":
            return HttpResponseForbidden(get_const("DUPLICATE_NAME_ERROR"))
        elif exceptionType == "InvalidData":
            return HttpResponseForbidden(get_const("NO_SUCH_FLOOR_ERROR"))
        else:
            return HttpResponseServerError(get_const("INTERNAL_ERROR"))


def createNewExhibit(request):
    data = json.loads(request.POST.get("jsonData"))
    return responseNewExhibit(thriftCommunicator.createNewExhibit(data))


def updateExhibit(request):
    PUT = QueryDict(request.body)
    data = json.loads(PUT.get("jsonData"))
    return responseNewExhibit(thriftCommunicator.updateExhibit(data))


def responseNewExhibit(newExhibit):
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


def removeExhibit(request):
    DELETE = QueryDict(request.body)
    data = json.loads(DELETE.get("jsonData"))
    exhibitId = data.get('exhibitId')
    thriftCommunicator.removeExhibit(exhibitId)
    return HttpResponse()


def getAllExhibits(request):
    return JsonResponse({
        'exhibits': _getExhibits()
    })


def getDialog(dialogName):
    dialogStructure = get_const(dialogName)
    contextDict = {
        'data': dialogStructure['data']
    }
    html = render_to_string('dialog/dialog.html', contextDict)
    retDict = {
        'data': dialogStructure,
        'html': html.replace("\n", "")
    }
    return JsonResponse(retDict)


def getColorPickerPopoverContent(request):
    colorsList = get_const("POPOVER_COLORS")
    popoverButtonHtml = render_to_string(
        'dialog/popoverColorsPicker.html', {'colorsList': colorsList})
    return JsonResponse({'html': popoverButtonHtml})


def getExhibitDialog(request):
    dialogStructure = get_const("EXHIBIT_DIALOG")
    availableFloors = get_const("AVAILABLE_FLOORS")[:]
    availableFloors.append("brak")
    dialogStructure['data'][1][1]['textList'] = availableFloors
    contextDict = {
        'data': dialogStructure['data']
    }
    html = render_to_string('dialog/dialog.html', contextDict)
    retDict = {
        'data': dialogStructure,
        'html': html.replace("\n", "")
    }
    return JsonResponse(retDict)


def getSimpleQuestionDialog(request):
    return getDialog("SIMPLE_QUESTION_DIALOG")


def getMultipleChoiceQuestionDialog(request):
    return getDialog("MULTIPLE_CHOICE_QUESTION_DIALOG")


def getExhibitPanel(request):
    availableFloors = get_const("AVAILABLE_FLOORS")
    html = render_to_string('exhibitPanel/exhibitPanel.html', {'availableFloors': availableFloors})
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
    changeImage = render_to_string('dialog/changeMap/changeImage.html', {'floor': floor})
    chooseMapChangeAction = render_to_string('dialog/changeMap/chooseMapChangeAction.html')
    addFloor = render_to_string('dialog/changeMap/addFloor.html')
    removeFloor = render_to_string('dialog/changeMap/removeFloor.html')
    processingMap = render_to_string('dialog/changeMap/processingMap.html')
    return JsonResponse({
            'addFloor': addFloor.replace("\n", ""),
            'removeFloor': removeFloor.replace("\n", ""),
            'processingMap': processingMap.replace("\n", ""),
            'chooseMapChangeAction': chooseMapChangeAction.replace("\n", ""),
            'changeImage': changeImage.replace("\n", "")})


def getChooseQuestionTypeDialog(request):
    html = render_to_string('dialog/chooseQuestionType.html')
    return JsonResponse({'html': html.replace("\n", "")})


def cloneExperimentDialog(request):
    return getDialog("CLONE_EXPERIMENT_DIALOG")


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
    'cloneExperimentDialog': cloneExperimentDialog
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


def _getReports(experimentId):
    reports = thriftCommunicator.getAllReportsForExperiment(int(experimentId))
    reportsList = list()
    for r in reports:
        reportsList.append({
            'reportId': r.reportId,
            'receiveDate': _dateToString(r.receiveDate)
        })
    return reportsList


def _newExperimentReadonlyPage(request):
    experimentId = request.GET.get("id")
    try:
        result = {
            'success': True,
            'readonlyExperimentActionRow': render_to_string('list/row/readonlyExperimentActionRow.html'),
            'readonlyExperimentQuestionRow': render_to_string('list/row/readonlyExperimentQuestionRow.html'),
            'reportRow': render_to_string('list/row/reportRow.html'),
            'experimentData': _getExperiment(experimentId),
            'reportsList': _getReports(experimentId),
            'activeExperiment': _getActiveExperiment(),
            'removeExperimentConfirmation': get_const("REMOVE_EXPERIMENT_CONFIRMATION"),
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
            'removeExperimentConfirmation': get_const("REMOVE_EXPERIMENT_CONFIRMATION"),
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


def question(request):
    try:
        if request.method == "POST":
            return createQuestion(request)
        elif request.method == "DELETE":
            return removeQuestion(request)
    except Exception as ex:
        exceptionType = type(ex).__name__
        if exceptionType == "DuplicateName":
            return HttpResponseForbidden(get_const("DUPLICATE_NAME_ERROR"))
        elif exceptionType == "InvalidData":
            return HttpResponseForbidden(get_const("INVALID_DATA_ERROR"))
        elif exceptionType == "ElementInUse":
            return HttpResponseForbidden(get_const("QUESTION_IN_USE_ERROR"))
        else:
            return HttpResponseServerError(get_const("INTERNAL_ERROR"))


def createQuestion(request):
    data = json.loads(request.POST.get("jsonData"))
    questionType = data.get('type')
    if questionType == QuestionType.SIMPLE.value:
        question = thriftCommunicator.createSimpleQuestion(data)
    elif questionType == QuestionType.SORT.value:
        question = thriftCommunicator.createSortQuestion(data)
    elif questionType == QuestionType.MULTIPLE.value:
        question = thriftCommunicator.createMultipleChoiceQuestion(data)
    return JsonResponse({
        'questionsList': _parseQuestions(
            thriftCommunicator.getAllQuestions(),
            question.questionId,
            questionType
        )
    })


def removeQuestion(request):
    DELETE = QueryDict(request.body)
    data = json.loads(DELETE.get("jsonData"))
    questionType = data.get('type')
    questionId = data.get('questionId')
    if questionType == QuestionType.SIMPLE.value:
        thriftCommunicator.removeSimpleQuestion(questionId)
    elif questionType == QuestionType.SORT.value:
        thriftCommunicator.removeSortQuestion(questionId)
    elif questionType == QuestionType.MULTIPLE.value:
        thriftCommunicator.removeMultipleChoiceQuestion(questionId)
    result = {
        'questionsList': _parseQuestions(thriftCommunicator.getAllQuestions())
    }
    return JsonResponse(result)



def action(request):
    try:
        if request.method == "POST":
            return createAction(request)
        else:
            return removeAction(request)
    except Exception as ex:
        exceptionType = type(ex).__name__
        if exceptionType == "DuplicateName":
            return HttpResponseForbidden(get_const("DUPLICATE_NAME_ERROR"))
        elif exceptionType == "InvalidData":
            return HttpResponseForbidden(get_const("INVALID_DATA_ERROR"))
        elif exceptionType == "ElementInUse":
            return HttpResponseForbidden(get_const("ACTION_IN_USE_ERROR"))
        else:
            return HttpResponseServerError(get_const("INTERNAL_ERROR"))


def createAction(request):
    data = json.loads(request.POST.get("jsonData"))
    action = thriftCommunicator.createAction(data)
    return JsonResponse({
        'actionsList': _parseActions(
            thriftCommunicator.getAllActions(),
            action.actionId
        )
    })


def removeAction(request):
    DELETE = QueryDict(request.body)
    actionId = json.loads(DELETE.get('jsonData'))['id']
    thriftCommunicator.removeAction(actionId)
    return JsonResponse({
        'actionsList': _parseActions(thriftCommunicator.getAllActions())
    })


def experiment(request):
    try:
        if request.method == "POST":
            return createExperiment(request)
        elif request.method == "PUT":
            return updateExperiment(request)
    except Exception as ex:
        if type(ex).__name__ == "DuplicateName":
            return HttpResponseForbidden(get_const("DUPLICATE_NAME_ERROR"))
        else:
            return HttpResponseServerError(get_const("INTERNAL_ERROR"))


def createExperiment(request):
    data = json.loads(request.POST.get("jsonData"))
    thriftCommunicator.createExperiment(data)
    return JsonResponse({
        "redirect": '/badania'
    })


def updateExperiment(request):
    PUT = QueryDict(request.body)
    data = json.loads(PUT.get("jsonData"))
    thriftCommunicator.updateExperiment(data)
    return JsonResponse({
        "redirect": '/badania'
    })


def _dateToString(date):
    repairNum = lambda x: "0{}".format(x) if x < 10 else "{}".format(x)
    return "{}/{}/{}".format(repairNum(date.day), repairNum(date.month), date.year)


def _parseExperiments(experiments):
    experimentsList = list()
    for e in experiments:
        experimentsList.append({
            'experimentId': e.id,
            'name': e.name,
            'startDate': _dateToString(e.startDate) if not e.startDate is None else None,
            'finishDate': _dateToString(e.finishDate) if not e.finishDate is None else None
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
            'startExperimentConfirmation': get_const("START_EXPERIMENT_CONFIRMATION"),
            'finishExperimentConfirmation': get_const("FINISH_EXPERIMENT_CONFIRMATION"),
            'tableList': render_to_string('list/dataList.html')}
    except Exception as ex:
        context = {
            'success': False,
            'message': str(ex)
        }
    return HttpResponse(template.render(context))


def activeExperiment(request):
    try:
        if request.method == "POST":
            return startExperiment(request)
        elif request.method == "DELETE":
            return finishExperiment(request)
    except Exception as ex:
        exceptionType = type(ex).__name__
        if exceptionType == "InvalidData":
            return HttpResponseForbidden(get_const("INVALID_DATA_ERROR"))
        else:
            return HttpResponseServerError(get_const("INTERNAL_ERROR"))


def startExperiment(request):
    data = json.loads(request.POST.get("jsonData"))
    experimentId = data.get('experimentId')
    thriftCommunicator.startExperiment(experimentId)
    return HttpResponse()


def finishExperiment(request):
    thriftCommunicator.finishExperiment()
    return HttpResponse()


def cloneExperiment(request):
    data = json.loads(request.POST.get("jsonData"))
    try:
        thriftCommunicator.cloneExperiment(
            data['experimentId'], data['newName'])
        return HttpResponse()
    except Exception as ex:
        exceptionType = type(ex).__name__
        if exceptionType == 'DuplicateName':
            return HttpResponseForbidden(get_const("DUPLICATE_NAME_ERROR"))
        elif exceptionType == "InvalidData":
            return HttpResponseServerError(get_const("INVALID_DATA_ERROR"))
        else:
            return HttpResponseServerError(get_const("INTERNAL_ERROR"))


def reportError(request):
    data = json.loads(request.POST.get("jsonData"))
    logger = logging.getLogger('jsLogger')
    logger.error(
        ": {0} in {1}, {2}".format(
            data['url'],
            data['lineNumber'],
            data['errorMsg']))
    return JsonResponse({})


def getReport(request):
    reportId = request.GET.get("id")
    try:
        filename = thriftCommunicator.getExcelReport(int(reportId))
        return _getReportFile(filename)
    except Exception as ex:
        if type(ex).__name__ == 'InvalidData':
            message = "{} {}.".format(
                get_const("NO_SUCH_REPORT_ERROR"), reportId)
        else:
            message = "{} ({})".format(get_const("INTERNAL_ERROR"), str(ex))
        return HttpResponse(message)


def getAllReports(request):
    experimentId = request.GET.get("id")
    try:
        filename = thriftCommunicator.getCombinedExcelReport(int(experimentId))
        return _getReportFile(filename)
    except Exception as ex:
        if type(ex).__name__ == 'InvalidData':
            message = "{} {}.".format(
                get_const("NO_SUCH_EXPERIMENT_ERROR"), experimentId)
        else:
            message = "{} ({})".format(get_const("INTERNAL_ERROR"), str(ex))
        return HttpResponse(message)


def _getReportFile(filename):
    path = os.path.join(get_const("EXCEL_FILES_ROOT"), filename)
    myStream = StringIO.StringIO()
    myStream.write(open(path, 'rb').read())
    os.remove(path)
    response = HttpResponse(content_type='application/x-zip-compressed')
    response[
        'Content-Disposition'] = "attachment; filename={}".format(filename)
    response['Content-Length'] = myStream.tell()
    response.write(myStream.getvalue())
    return response


def removeFloor(request):
    floor = int(request.POST.get("floor"))
    try:
        thriftCommunicator.removeFloor(floor)
        result = {
            'success': True
        }
    except Exception as ex:
        if type(ex).__name__ == 'InvalidData':
            message = "{} {}".format(get_const("NO_SUCH_FLOOR_ERROR"), floor)
        else:
            message = "{} ({})".format(get_const("INTERNAL_ERROR"), str(ex))
        result = {
            'success': False,
            'message': message
        }
    return JsonResponse(result)


def removeExperiment(request):
    data = json.loads(request.POST.get("jsonData"))
    experimentId = data.get('experimentId')
    try:
        thriftCommunicator.removeExperiment(experimentId)
        return JsonResponse({
            "redirect": "/badania/"
        })
    except Exception as ex:
        exceptionType = type(ex).__name__
        if exceptionType == "InvalidData":
            return HttpResponseForbidden(get_const("INVALID_DATA_ERROR"))
        else:
            return HttpResponseServerError(get_const("INTERNAL_ERROR"))
