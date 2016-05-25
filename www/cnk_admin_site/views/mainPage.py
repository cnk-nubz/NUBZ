import os
os.environ['DJANGO_SETTINGS_MODULE'] = 'cnk.settings'

from django.conf                  import settings
from django.http                  import HttpResponse, HttpResponseRedirect, HttpResponseServerError
from django.views.decorators.csrf import ensure_csrf_cookie
from django.template              import RequestContext, loader
from django.template.loader       import render_to_string
from utils                        import *
from enums                        import ActiveLink
from ..thrift_communication       import ThriftCommunicator, toThrift, fromThrift
import HTMLStructures as HTMLStructures
thriftCommunicator = ThriftCommunicator.ThriftCommunicator()


def index(request):
    if request.META.get("HTTP_USER_AGENT", '').lower().find("android") > 0:
        return HttpResponseRedirect("/static/android_app/example")
    return getMapPage(request, 'map/justMap.html', ActiveLink.JUST_MAP.value)


def editMapPage(request):
    return getMapPage(request, 'map/editMap.html', ActiveLink.EDIT_MAP.value)


@startingPage
@ensure_csrf_cookie
def getMapPage(request, file, activeLink):
    floorTilesInfo = getMapImageInfo()
    exhibits = map(fromThrift.exhibit, thriftCommunicator.getAllExhibits())
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


@ensure_csrf_cookie
@startingPage
def newExperimentPage(request):
    readonly = request.GET.get("readonly")
    if not (readonly is None):
        return _newExperimentReadonlyPage(request)
    experimentId = maybe(int, request.GET.get("id"))
    questionsList = fromThrift.questionsList(thriftCommunicator.getAllQuestions())
    result = {
        'experimentData': maybe(
            compose(fromThrift.experiment, thriftCommunicator.getExperiment), experimentId),
        'questionsList': mergeQuestions(questionsList),
        'actionsList': map(fromThrift.action, thriftCommunicator.getAllActions()),
        'structures': HTMLStructures.newExperimentPage()}
    template = loader.get_template('newExperimentPage.html')
    return HttpResponse(template.render(RequestContext(request, result)))


@ensure_csrf_cookie
@startingPage
def _newExperimentReadonlyPage(request):
    experimentId = maybe(int, request.GET.get("id"))
    activeExperiment = fromThrift.singleExperimentInfo(thriftCommunicator.getActiveExperiment())
    reports = thriftCommunicator.getAllReportsForExperiment(experimentId)
    reportsList = map(fromThrift.reportInfo, reports)
    result = {
        'experimentData': maybe(
            compose(fromThrift.experiment, thriftCommunicator.getExperiment), experimentId),
        'reportsList': reportsList,
        'activeExperiment': activeExperiment,
        'structures': HTMLStructures.newExperimentReadonlyPage()}
    template = loader.get_template('readonlyExperiment.html')
    return HttpResponse(template.render(RequestContext(request, result)))


@ensure_csrf_cookie
@startingPage
def experimentsPage(request):
    template = loader.get_template('experimentsPage.html')
    readyExperiments = map(fromThrift.experimentInfo, thriftCommunicator.getReadyExperiments())
    finishedExperiments = map(fromThrift.experimentInfo, thriftCommunicator.getFinishedExperiments())
    activeExperiment = fromThrift.singleExperimentInfo(thriftCommunicator.getActiveExperiment())
    context = {
        'activeLink': ActiveLink.EXPERIMENTS_PAGE.value,
        'finishedExperiments': finishedExperiments,
        'readyExperiments': readyExperiments,
        'activeExperiment': activeExperiment,
        'structures': HTMLStructures.experimentsPage()}
    return HttpResponse(template.render(RequestContext(request, context)))


@ensure_csrf_cookie
@startingPage
def questionsAndActionsPage(request):
    questionsList = fromThrift.questionsList(thriftCommunicator.getAllQuestions())
    context = {
        'activeLink': ActiveLink.QUESTIONS_ACTIONS_PAGE.value,
        'questionsList': mergeQuestions(questionsList),
        'actionsList': map(fromThrift.action, thriftCommunicator.getAllActions()),
        'structures': HTMLStructures.questionsAndActionsPage()}
    template = loader.get_template('questionsAndActionsPage.html')
    return HttpResponse(template.render(RequestContext(request, context)))


def androidApp(request):
    return HttpResponseRedirect("/static/android_app/example")
