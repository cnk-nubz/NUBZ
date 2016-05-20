import os
os.environ['DJANGO_SETTINGS_MODULE'] = 'cnk.settings'

from django.conf import settings
from django.http import HttpResponse, HttpResponseRedirect
from django.views.decorators.csrf import ensure_csrf_cookie
from django.template import RequestContext, loader
from django.template.loader import render_to_string

from utils import *
import HTMLStructures as HTMLStructures
from enums import ActiveLink
from ..thrift_communication import ThriftCommunicator, toThrift, fromThrift
thriftCommunicator = ThriftCommunicator.ThriftCommunicator()


def index(request):
    if request.META.get("HTTP_USER_AGENT", '').lower().find("android") > 0:
        return HttpResponseRedirect("/static/android_app/example")
    return getMapPage(request, 'map/justMap.html', ActiveLink.JUST_MAP.value)


def editMapPage(request):
    return getMapPage(request, 'map/editMap.html', ActiveLink.EDIT_MAP.value)


@ensure_csrf_cookie
def getMapPage(request, file, activeLink):
    try:
        floorTilesInfo = getMapImageInfo()
        exhibits = map(fromThrift.exhibit, thriftCommunicator.getAllExhibits())
    except Exception as ex:
        return handleException(ex)

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
        'availableFloors': availableFloors,
        'structures': HTMLStructures.getMapPage()
    })
    return HttpResponse(template.render(context))


def newExperimentPage(request):
    readonly = request.GET.get("readonly")
    if not (readonly is None):
        return _newExperimentReadonlyPage(request)
    experimentId = maybe(int, request.GET.get("id"))
    try:
        questionsList = fromThrift.questionsList(thriftCommunicator.getAllQuestions())
        result = {
            'success': True,
            'experimentData': maybe(
                compose(fromThrift.experiment, thriftCommunicator.getExperiment), experimentId),
            'questionsList': mergeQuestions(questionsList),
            'actionsList': map(fromThrift.action, thriftCommunicator.getAllActions()),
            'structures': HTMLStructures.newExperimentPage()}
    except Exception as ex:
        result = {
            'success': False,
            'message': str(ex)
        }
    template = loader.get_template('newExperimentPage.html')
    return HttpResponse(template.render(RequestContext(request, result)))


def _newExperimentReadonlyPage(request):
    experimentId = maybe(int, request.GET.get("id"))
    activeExperiment = fromThrift.singleExperimentInfo(thriftCommunicator.getActiveExperiment())
    reports = thriftCommunicator.getAllReportsForExperiment(experimentId)
    reportsList = map(fromThrift.reportInfo, reports)
    result = {
        'success': True,
        'experimentData': maybe(
            compose(fromThrift.experiment, thriftCommunicator.getExperiment), experimentId),
        'reportsList': reportsList,
        'activeExperiment': activeExperiment,
        'structures': HTMLStructures.newExperimentReadonlyPage()}

    template = loader.get_template('readonlyExperiment.html')
    return HttpResponse(template.render(RequestContext(request, result)))


@ensure_csrf_cookie
def experimentsPage(request):
    template = loader.get_template('experimentsPage.html')
    readyExperiments = map(fromThrift.experimentInfo, thriftCommunicator.getReadyExperiments())
    finishedExperiments = map(fromThrift.experimentInfo, thriftCommunicator.getFinishedExperiments())
    activeExperiment = fromThrift.singleExperimentInfo(thriftCommunicator.getActiveExperiment())
    try:
        context = {
            'success': True,
            'activeLink': ActiveLink.EXPERIMENTS_PAGE.value,
            'finishedExperiments': finishedExperiments,
            'readyExperiments': readyExperiments,
            'activeExperiment': activeExperiment,
            'structures': HTMLStructures.experimentsPage()}
    except Exception as ex:
        context = {
            'success': False,
            'message': str(ex)
        }
    return HttpResponse(template.render(context))


def questionsAndActionsPage(request):
    questionsList = fromThrift.questionsList(thriftCommunicator.getAllQuestions())
    try:
        result = {
            'success': True,
            'activeLink': ActiveLink.QUESTIONS_ACTIONS_PAGE.value,
            'questionsList': mergeQuestions(questionsList),
            'actionsList': map(fromThrift.action, thriftCommunicator.getAllActions()),
            'structures': HTMLStructures.questionsAndActionsPage()}
    except Exception as ex:
        result = {
            'success': False,
            'exceptionType': type(ex).__name__,
            'activeLink': ActiveLink.QUESTIONS_ACTIONS_PAGE.value,
            'message': str(ex)
        }
    template = loader.get_template('questionsAndActionsPage.html')
    return HttpResponse(template.render(RequestContext(request, result)))


def androidApp(request):
    return HttpResponseRedirect("/static/android_app/example")
